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

#include <d3d9.h>
#include <vector>
#include "Helper.h"

#include "D3D9Geometry.h"


//
// CD3D9Dots
//

CD3D9Dots::~CD3D9Dots()
{
	InvalidateDeviceObjects();
}

HRESULT CD3D9Dots::InitDeviceObjects(IDirect3DDevice9* pDevice)
{
	InvalidateDeviceObjects();
	if (!pDevice) {
		return E_POINTER;
	}

	m_pDevice = pDevice;
	m_pDevice->AddRef();

	return S_OK;
}

void CD3D9Dots::InvalidateDeviceObjects()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pVertexBuffer);
}

void CD3D9Dots::ClearPoints()
{
	m_Vertices.clear();
	m_bAlphaBlend = false;
}

bool CD3D9Dots::AddPoints(POINT* poins, const UINT size, const D3DCOLOR color)
{
	if (!CheckNumPoints(size)) {
		return false;
	}

	m_bAlphaBlend = (color >> 24) < 0xFF;

	auto pos = m_Vertices.size();
	m_Vertices.resize(pos + size);

	while (pos < m_Vertices.size()) {
		m_Vertices[pos++] = { {(float)(*poins).x, (float)(*poins).y, 0.f, 1.f}, color };
		poins++;
	}

	return true;
}

bool CD3D9Dots::AddGFPoints(
	int Xstart, int Xstep,
	int Yaxis, int* Ydata, UINT Yoffset,
	const UINT size, const D3DCOLOR color)
{
	if (!CheckNumPoints(size)) {
		return false;
	}

	m_bAlphaBlend = (color >> 24) < 0xFF;

	auto pos = m_Vertices.size();
	m_Vertices.resize(pos + size);

	while (pos < m_Vertices.size()) {
		m_Vertices[pos++] = { {(float)Xstart, (float)(Yaxis - Ydata[Yoffset++]), 0.f, 1.f}, color };
		Xstart += Xstep;
		if (Yoffset == size) {
			Yoffset = 0;
		}
	}

	return true;
}

HRESULT CD3D9Dots::UpdateVertexBuffer()
{
	HRESULT hr = S_FALSE;
	UINT vertexSize = m_Vertices.size() * sizeof(POINTVERTEX);

	if (m_pVertexBuffer) {
		D3DVERTEXBUFFER_DESC desc;
		hr = m_pVertexBuffer->GetDesc(&desc);
		if (FAILED(hr) || desc.Size < vertexSize) {
			SAFE_RELEASE(m_pVertexBuffer);
		}
	}

	if (!m_pVertexBuffer) {
		hr = m_pDevice->CreateVertexBuffer(vertexSize, 0, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &m_pVertexBuffer, nullptr);
	}

	if (m_pVertexBuffer) {
		VOID* pVertices;
		hr = m_pVertexBuffer->Lock(0, vertexSize, (void**)&pVertices, 0);
		if (S_OK == hr) {
			memcpy(pVertices, m_Vertices.data(), vertexSize);
			m_pVertexBuffer->Unlock();
		};
	}

	return hr;
}

HRESULT CD3D9Dots::Draw()
{
	if (m_bAlphaBlend) {
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else {
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
	m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	HRESULT hr = m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(POINTVERTEX));
	if (S_OK == hr) {
		hr = m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		hr = DrawPrimitive();
	}

	return hr;
}
