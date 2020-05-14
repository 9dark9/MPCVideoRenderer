/*
* (C) 2018-2020 see Authors.txt
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

#pragma once

#include <d3d9.h>
#include <dxva2api.h>
#include "csputils.h"

#ifndef FCC
#define FCC(ch4) ((((DWORD)(ch4) & 0xFF) << 24) |     \
                  (((DWORD)(ch4) & 0xFF00) << 8) |    \
                  (((DWORD)(ch4) & 0xFF0000) >> 8) |  \
                  (((DWORD)(ch4) & 0xFF000000) >> 24))
#endif

#define D3DFMT_YV12 (D3DFORMAT)FCC('YV12')
#define D3DFMT_NV12 (D3DFORMAT)FCC('NV12')
#define D3DFMT_P010 (D3DFORMAT)FCC('P010')
#define D3DFMT_P016 (D3DFORMAT)FCC('P016')
#define D3DFMT_YV16 (D3DFORMAT)FCC('YV16')
#define D3DFMT_P210 (D3DFORMAT)FCC('P210')
#define D3DFMT_P216 (D3DFORMAT)FCC('P216')
#define D3DFMT_YV24 (D3DFORMAT)FCC('YV24')
#define D3DFMT_AYUV (D3DFORMAT)FCC('AYUV')
#define D3DFMT_Y410 (D3DFORMAT)FCC('Y410')
#define D3DFMT_Y416 (D3DFORMAT)FCC('Y416')

#define DXGI_FORMAT_PLANAR (DXGI_FORMAT)0xFFFF

#define PCIV_AMDATI      0x1002
#define PCIV_NVIDIA      0x10DE
#define PCIV_INTEL       0x8086

#ifdef _DEBUG
#define DLog(...) DbgLogInfo(LOG_TRACE, 3, __VA_ARGS__)
#define DLogIf(f,...) {if (f) DbgLogInfo(LOG_TRACE, 3, __VA_ARGS__);}
#define DLogError(...) DbgLogInfo(LOG_ERROR, 3, __VA_ARGS__)
#else
#define DLog(...) __noop
#define DLogIf(f,...) __noop
#define DLogError(...) __noop
#endif

#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = nullptr; } }
#define SAFE_CLOSE_HANDLE(p) { if (p) { if ((p) != INVALID_HANDLE_VALUE) ASSERT(CloseHandle(p)); (p) = nullptr; } }
#define SAFE_DELETE(p)       { if (p) { delete (p); (p) = nullptr; } }

#define QI(i) (riid == __uuidof(i)) ? GetInterface((i*)this, ppv) :

#define ALIGN(x, a)           __ALIGN_MASK(x,(decltype(x))(a)-1)
#define __ALIGN_MASK(x, mask) (((x)+(mask))&~(mask))

DEFINE_GUID(MEDIASUBTYPE_Y8,     0x20203859, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_Y800,   0x30303859, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_Y16,    0x10003159, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71); // Y1[0][16]
DEFINE_GUID(MEDIASUBTYPE_YV16,   0x36315659, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_YV24,   0x34325659, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_Y410,   0x30313459, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_Y416,   0x36313459, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_RGB48,  0x30424752, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71); // BGR[48] (BGR0)
DEFINE_GUID(MEDIASUBTYPE_ARGB64, 0x40415242, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71); // BRA[64] (BRA@)
DEFINE_GUID(MEDIASUBTYPE_b48r,   0x72383462, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(MEDIASUBTYPE_b64a,   0x61343662, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);

DEFINE_GUID(CLSID_XySubFilter,            0x2DFCB782, 0xEC20, 0x4A7C, 0xB5, 0x30, 0x45, 0x77, 0xAD, 0xB3, 0x3F, 0x21);
DEFINE_GUID(CLSID_XySubFilter_AutoLoader, 0x6B237877, 0x902B, 0x4C6C, 0x92, 0xF6, 0xE6, 0x31, 0x69, 0xA5, 0x16, 0x6C);

#define VIDEOTRANSFERMATRIX_BT2020_10 4
#define VIDEOTRANSFERMATRIX_BT2020_12 5
#define VIDEOTRANSFERMATRIX_FCC       6 // non-standard
#define VIDEOTRANSFERMATRIX_YCgCo     7 // non-standard

#define VIDEOPRIMARIES_BT2020  9
#define VIDEOPRIMARIES_XYZ    10
#define VIDEOPRIMARIES_DCI_P3 11
#define VIDEOPRIMARIES_ACES   12

#define VIDEOTRANSFUNC_Log_100     9
#define VIDEOTRANSFUNC_Log_316    10
#define VIDEOTRANSFUNC_709_sym    11
#define VIDEOTRANSFUNC_2020_const 12
#define VIDEOTRANSFUNC_2020       13
#define VIDEOTRANSFUNC_26         14
#define VIDEOTRANSFUNC_2084       15
#define VIDEOTRANSFUNC_HLG        16
#define VIDEOTRANSFUNC_10_rel     17

struct VR_Extradata {
	LONG  QueryWidth;
	LONG  QueryHeight;
	LONG  FrameWidth;
	LONG  FrameHeight;
	DWORD Compression;
};

struct ScalingShaderResId {
	UINT shaderX;
	UINT shaderY;
	wchar_t* const description;
};

template <typename T>
// If the specified value is out of range, set to default values.
inline T discard(T const& val, T const& def, T const& lo, T const& hi)
{
	return (val > hi || val < lo) ? def : val;
}

template <typename T>
inline T round_pow2(T number, T pow2)
{
	ASSERT(pow2 > 0);
	ASSERT(!(pow2 & (pow2 - 1)));
	--pow2;
	if (number < 0) {
		return (number - pow2) & ~pow2;
	} else {
		return (number + pow2) & ~pow2;
	}
}

LPCWSTR GetWindowsVersion();
LPCWSTR GetNameAndVersion();

inline CStringW CStringFromGUID(const GUID& guid)
{
	WCHAR buff[40] = {};
	if (StringFromGUID2(guid, buff, 39) <= 0) {
		StringFromGUID2(GUID_NULL, buff, 39);
	}
	return CStringW(buff);
}

CStringW HR2Str(const HRESULT hr);
CStringW MediaType2Str(const CMediaType *pmt);

const wchar_t* D3DFormatToString(const D3DFORMAT format);
const wchar_t* DXGIFormatToString(const DXGI_FORMAT format);
const wchar_t* DXVA2VPDeviceToString(const GUID& guid);
void SetDefaultDXVA2ProcAmpRanges(DXVA2_ValueRange(&DXVA2ProcAmpRanges)[4]);
void SetDefaultDXVA2ProcAmpValues(DXVA2_ProcAmpValues& DXVA2ProcAmpValues);
bool IsDefaultDXVA2ProcAmpValues(const DXVA2_ProcAmpValues& DXVA2ProcAmpValues);

typedef void(*CopyFrameDataFn)(const UINT lines, BYTE* dst, UINT dst_pitch, const BYTE* src, int src_pitch);

enum ColorFormat_t {
	CF_NONE = 0,
	CF_YV12,
	CF_NV12,
	CF_P010,
	CF_P016,
	CF_YUY2,
	CF_YV16,
	CF_P210,
	CF_P216,
	CF_YV24,
	CF_AYUV,
	CF_Y410,
	CF_Y416,
	CF_RGB24,
	CF_XRGB32,
	CF_ARGB32,
	CF_RGB48,
	CF_B48R,
	CF_ARGB64,
	CF_B64A,
	CF_Y8,
	CF_Y800,
	CF_Y116,
};

enum ColorSystem_t {
	CS_YUV,
	CS_RGB,
	CS_GRAY
};

struct DX9PlanarPrms_t {
	D3DFORMAT   FmtPlane1;
	D3DFORMAT   FmtPlane2;
	D3DFORMAT   FmtPlane3;
	UINT        div_chroma_w;
	UINT        div_chroma_h;
};

struct DX11PlanarPrms_t {
	DXGI_FORMAT FmtPlane1;
	DXGI_FORMAT FmtPlane2;
	DXGI_FORMAT FmtPlane3;
	UINT        div_chroma_w;
	UINT        div_chroma_h;
};

struct FmtConvParams_t {
	ColorFormat_t     cformat;
	GUID              Subtype;
	char*             str;
	D3DFORMAT         DXVA2Format;
	D3DFORMAT         D3DFormat;
	DX9PlanarPrms_t*  pDX9Planes;
	DXGI_FORMAT       VP11Format;
	DXGI_FORMAT       DX11Format;
	DX11PlanarPrms_t* pDX11Planes;
	int               Packsize;
	int               PitchCoeff;
	ColorSystem_t     CSType;
	int               Subsampling;
	int               CDepth;
	CopyFrameDataFn   Func;
	CopyFrameDataFn   FuncSSSE3;
};

const FmtConvParams_t& GetFmtConvParams(const ColorFormat_t fmt);
const FmtConvParams_t& GetFmtConvParams(const GUID subtype);
CopyFrameDataFn GetCopyFunction(const FmtConvParams_t& params);

// YUY2, AYUV, RGB32 to D3DFMT_X8R8G8B8, ARGB32 to D3DFMT_A8R8G8B8
void CopyFrameAsIs(const UINT lines, BYTE* dst, UINT dst_pitch, const BYTE* src, int src_pitch);
// RGB24 to D3DFMT_X8R8G8B8
void CopyFrameRGB24(const UINT lines, BYTE* dst, UINT dst_pitch, const BYTE* src, int src_pitch);
void CopyRGB24_SSSE3(const UINT lines, BYTE* dst, UINT dst_pitch, const BYTE* src, int src_pitch); // 30% faster than CopyFrameRGB24().
// RGB48, b48r to D3DFMT_A16B16G16R16
void CopyFrameRGB48(const UINT lines, BYTE* dst, UINT dst_pitch, const BYTE* src, int src_pitch);
void CopyRGB48_SSSE3(const UINT lines, BYTE* dst, UINT dst_pitch, const BYTE* src, int src_pitch); // Not faster than CopyFrameRGB48().
// b64a to D3DFMT_A16B16G16R16
void CopyFrameB64A(const UINT lines, BYTE* dst, UINT dst_pitch, const BYTE* src, int src_pitch);
// YV12
void CopyFrameYV12(const UINT lines, BYTE* dst, UINT dst_pitch, const BYTE* src, int src_pitch);
// Y410 (not used)
void CopyFrameY410(const UINT lines, BYTE* dst, UINT dst_pitch, const BYTE* src, int src_pitch);

void ClipToSurface(const int texW, const int texH, RECT& s, RECT& d);

void set_colorspace(const DXVA2_ExtendedFormat& extfmt, mp_colorspace& colorspace);

BITMAPINFOHEADER* GetBIHfromVIHs(const AM_MEDIA_TYPE* mt);

HRESULT SaveARGB32toBMP(BYTE* src, const UINT src_pitch, const UINT width, const UINT height, const wchar_t* filename);

DXVA2_ExtendedFormat SpecifyExtendedFormat(DXVA2_ExtendedFormat exFormat, const FmtConvParams_t& fmtParams, const UINT width, const UINT height);

void GetExtendedFormatString(LPCSTR (&strs)[6], const DXVA2_ExtendedFormat exFormat, const ColorSystem_t colorSystem);

HRESULT GetDataFromResource(LPVOID& data, DWORD& size, UINT resid);
