#pragma once
#include "../stdafx.h"
#include <d3d11.h>

namespace detour
{
	BOOL AttachD3D11Device(ID3D11Device *pDevice);
	BOOL DetachD3D11Device();
	BOOL AttachD3D11DeviceContext(ID3D11DeviceContext *pDeviceContext);
	BOOL DetachD3D11DeviceContext();
	BOOL AttachDXGISwapChain(IDXGISwapChain *pSwapChain);
	BOOL DetachDXGISwapChain();
	BOOL AttachDXGIDevice(IDXGIDevice *pDevice);
	BOOL DetachDXGIDevice();
	BOOL AttachDXGIAdapter(IDXGIAdapter *pAdapter);
	BOOL DetachDXGIAdapter();
	BOOL AttachDXGIFactory(IDXGIFactory *pFactory);
	BOOL DetachDXGIFactory();

	DLLEXPORT BOOL AttachD3D11();
	DLLEXPORT BOOL DetachD3D11();
}