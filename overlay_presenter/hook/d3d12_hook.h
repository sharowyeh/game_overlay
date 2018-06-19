#pragma once
#include "../stdafx.h"
#include "dxgi_hook.h"
#include <d3d12.h>

namespace hook
{
	class D3D12Hook
	{
	public:
		//test
		void D3D12DrawOverlay(unsigned char *pbData, DXGI_HOOK_RECT rect);
		HRESULT CompileShaderV4_0(const char *entry, const char *target, ID3DBlob **ppCode);
		HRESULT D3D12CreateResource();
		HRESULT D3D12ReleaseResource();
	private:
		IDXGISwapChain *m_pSwapChain = NULL;
		ID3D12Device *m_pDevice = NULL;
	};
}