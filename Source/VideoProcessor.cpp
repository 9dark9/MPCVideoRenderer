/*
 * (C) 2020 see Authors.txt
 *
 * This file is part of MPC-BE.
 *
 * MPC-BE is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPC-BE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "stdafx.h"

#include <Mferror.h>
#include "Helper.h"
#include "VideoRenderer.h"

#include "VideoProcessor.h"

HRESULT CVideoProcessor::GetVideoSize(long *pWidth, long *pHeight)
{
	CheckPointer(pWidth, E_POINTER);
	CheckPointer(pHeight, E_POINTER);

	if (m_iRotation == 90 || m_iRotation == 270) {
		*pWidth  = m_srcRectHeight;
		*pHeight = m_srcRectWidth;
	} else {
		*pWidth  = m_srcRectWidth;
		*pHeight = m_srcRectHeight;
	}

	return S_OK;
}

HRESULT CVideoProcessor::GetAspectRatio(long *plAspectX, long *plAspectY)
{
	CheckPointer(plAspectX, E_POINTER);
	CheckPointer(plAspectY, E_POINTER);

	if (m_iRotation == 90 || m_iRotation == 270) {
		*plAspectX = m_srcAspectRatioY;
		*plAspectY = m_srcAspectRatioX;
	} else {
		*plAspectX = m_srcAspectRatioX;
		*plAspectY = m_srcAspectRatioY;
	}

	return S_OK;
}

void CVideoProcessor::SetTexFormat(int value)
{
	switch (value) {
	case TEXFMT_AUTOINT:
	case TEXFMT_8INT:
	case TEXFMT_10INT:
	case TEXFMT_16FLOAT:
		m_iTexFormat = value;
		break;
	default:
		DLog(L"CVideoProcessor::SetTexFormat() unknown value {}", value);
		ASSERT(FALSE);
		return;
	}
}

void CVideoProcessor::CalcStatsFont()
{
	if (m_iResizeStats == 1) {
		int w = std::max(512, m_windowRect.Width() / 2 - 10) - 5 - 3;
		int h = std::max(280, m_windowRect.Height() - 10) - 5 - 3;
		m_StatsFontH = (int)std::ceil(std::min(w / 36.0, h / 19.4));
		m_StatsFontH &= ~1;
		if (m_StatsFontH < 14) {
			m_StatsFontH = 14;
		}
	}
	else {
		m_StatsFontH = 14;
	}
}

bool CVideoProcessor::CheckGraphPlacement()
{
	return m_GraphRect.left >= 0 && m_GraphRect.top >= 0
		&& !(m_GraphRect.left < m_StatsRect.right && m_GraphRect.top < m_StatsRect.bottom);
}

void CVideoProcessor::CalcGraphParams()
{
	auto CalcGraphRect = [&]() {
		m_GraphRect.right = m_windowRect.right - 20;
		m_GraphRect.left  = m_GraphRect.right - m_Xstep * m_Syncs.Size();

		m_GraphRect.bottom = m_windowRect.bottom - 20;
		m_GraphRect.top    = m_GraphRect.bottom - 120 * m_Yscale;
	};

	m_Xstep = 4;
	m_Yscale = 2;
	CalcGraphRect();

	if (!CheckGraphPlacement()) {
		m_Xstep = 2;
		m_Yscale = 1;
		CalcGraphRect();
	}

	m_Yaxis = m_GraphRect.bottom - 50 * m_Yscale;
}

void CVideoProcessor::SetDisplayInfo(const DisplayConfig_t& displayConfig, const bool primary, const bool fullscreen)
{
	m_strStatsDispInfo = DisplayConfigToString(displayConfig);
	if (m_strStatsDispInfo.empty()) {
		m_strStatsDispInfo = D3DDisplayModeToString(*m_pDisplayMode);
	}
	if (primary) {
		m_strStatsDispInfo.append(L" [Primary]");
	}
	if (fullscreen) {
		m_strStatsDispInfo.append(L" fullscreen");
	} else {
		m_strStatsDispInfo.append(L" windowed");
	}
}

// IUnknown

STDMETHODIMP CVideoProcessor::QueryInterface(REFIID riid, void **ppv)
{
	if (!ppv) {
		return E_POINTER;
	}
	if (riid == IID_IUnknown) {
		*ppv = static_cast<IUnknown*>(static_cast<IMFVideoProcessor*>(this));
	}
	else if (riid == IID_IMFVideoProcessor) {
		*ppv = static_cast<IMFVideoProcessor*>(this);
	}
	else if (riid == IID_IMFVideoMixerBitmap) {
		*ppv = static_cast<IMFVideoMixerBitmap*>(this);
	}
	else {
		*ppv = nullptr;
		return E_NOINTERFACE;
	}
	AddRef();
	return S_OK;
}

STDMETHODIMP_(ULONG) CVideoProcessor::AddRef()
{
	return InterlockedIncrement(&m_nRefCount);
}

STDMETHODIMP_(ULONG) CVideoProcessor::Release()
{
	ULONG uCount = InterlockedDecrement(&m_nRefCount);
	if (uCount == 0) {
		delete this;
	}
	// For thread safety, return a temporary variable.
	return uCount;
}

// IMFVideoProcessor

STDMETHODIMP CVideoProcessor::GetProcAmpRange(DWORD dwProperty, DXVA2_ValueRange *pPropRange)
{
	CheckPointer(pPropRange, E_POINTER);
	if (m_srcParams.cformat == CF_NONE) {
		return MF_E_TRANSFORM_TYPE_NOT_SET;
	}

	switch (dwProperty) {
	case DXVA2_ProcAmp_Brightness: *pPropRange = m_DXVA2ProcAmpRanges[0]; break;
	case DXVA2_ProcAmp_Contrast:   *pPropRange = m_DXVA2ProcAmpRanges[1]; break;
	case DXVA2_ProcAmp_Hue:        *pPropRange = m_DXVA2ProcAmpRanges[2]; break;
	case DXVA2_ProcAmp_Saturation: *pPropRange = m_DXVA2ProcAmpRanges[3]; break;
	default:
		return E_INVALIDARG;
	}

	return S_OK;
}

STDMETHODIMP CVideoProcessor::GetProcAmpValues(DWORD dwFlags, DXVA2_ProcAmpValues *Values)
{
	CheckPointer(Values, E_POINTER);
	if (m_srcParams.cformat == CF_NONE) {
		return MF_E_TRANSFORM_TYPE_NOT_SET;
	}

	if (dwFlags & DXVA2_ProcAmp_Brightness) { Values->Brightness = m_DXVA2ProcAmpValues.Brightness; }
	if (dwFlags & DXVA2_ProcAmp_Contrast)   { Values->Contrast   = m_DXVA2ProcAmpValues.Contrast  ; }
	if (dwFlags & DXVA2_ProcAmp_Hue)        { Values->Hue        = m_DXVA2ProcAmpValues.Hue       ; }
	if (dwFlags & DXVA2_ProcAmp_Saturation) { Values->Saturation = m_DXVA2ProcAmpValues.Saturation; }

	return S_OK;
}

STDMETHODIMP CVideoProcessor::GetBackgroundColor(COLORREF *lpClrBkg)
{
	CheckPointer(lpClrBkg, E_POINTER);
	*lpClrBkg = RGB(0, 0, 0);
	return S_OK;
}

// IMFVideoMixerBitmap

STDMETHODIMP CVideoProcessor::ClearAlphaBitmap()
{
	CAutoLock cRendererLock(&m_pFilter->m_RendererLock);
	m_bAlphaBitmapEnable = false;

	return S_OK;
}

STDMETHODIMP CVideoProcessor::GetAlphaBitmapParameters(MFVideoAlphaBitmapParams *pBmpParms)
{
	CheckPointer(pBmpParms, E_POINTER);
	CAutoLock cRendererLock(&m_pFilter->m_RendererLock);

	if (m_bAlphaBitmapEnable) {
		pBmpParms->dwFlags      = MFVideoAlphaBitmap_SrcRect|MFVideoAlphaBitmap_DestRect;
		pBmpParms->clrSrcKey    = 0; // non used
		pBmpParms->rcSrc        = m_AlphaBitmapRectSrc;
		pBmpParms->nrcDest      = m_AlphaBitmapNRectDest;
		pBmpParms->fAlpha       = 0; // non used
		pBmpParms->dwFilterMode = D3DTEXF_LINEAR;
		return S_OK;
	} else {
		return MF_E_NOT_INITIALIZED;
	}
}
