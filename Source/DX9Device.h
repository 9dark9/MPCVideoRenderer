/*
* (C) 2019-2022 see Authors.txt
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

#include <thread>

class CDX9Device
{
private:
	CComPtr<IDirect3D9Ex>            m_pD3DEx;
	CComPtr<IDirect3DDeviceManager9> m_pD3DDeviceManager;
	UINT    m_nResetTocken = 0;

	UINT m_nCurrentAdapter9 = D3DADAPTER_DEFAULT;
	HWND m_hDX9Wnd = nullptr;

	CAMEvent m_evInit;
	CAMEvent m_evQuit;
	CAMEvent m_evThreadFinishJob;
	HRESULT m_hrThread = E_FAIL;
	std::thread m_deviceThread;
	void DeviceThreadFunc();

	HRESULT InitDX9DeviceInternal();

protected:
	D3DDISPLAYMODEEX m_DisplayMode = { sizeof(D3DDISPLAYMODEEX) };
	CComPtr<IDirect3DDevice9Ex> m_pD3DDevEx;
	D3DFORMAT m_srcDXVA2Format = D3DFMT_UNKNOWN;
	D3DPRESENT_PARAMETERS m_d3dpp = {};

	CDX9Device();
	~CDX9Device();

	IDirect3DDeviceManager9* GetDevMan9() { return m_pD3DDeviceManager; }

	HRESULT InitDX9Device(const HWND hwnd);
	void ReleaseDX9Device();
};
