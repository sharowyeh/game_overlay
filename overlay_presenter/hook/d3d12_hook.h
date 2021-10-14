#pragma once
#include "../stdafx.h"
#include "dxgi_hook.h"
#include <d3d12.h>

namespace hook
{
	class D3D12Hook
	{
	public:
		D3D12Hook(IDXGISwapChain *pSwapChain);
		virtual ~D3D12Hook();
		//test
		void D3D12DrawOverlay(unsigned char *pbData, DXGI_HOOK_RECT rect);
		HRESULT D3D12CreateResource();
		HRESULT D3D12ReleaseResource();

	private:
		IDXGISwapChain *m_pSwapChain = NULL;
		ID3D12Device *m_pDevice = NULL;
		ID3D12CommandAllocator *m_pCommandAllocator = NULL;
		ID3D12CommandQueue *m_pCommandQueue = NULL;
		ID3D12RootSignature *m_pRootSignature = NULL;
		ID3D12PipelineState *m_pPipelineState = NULL;
		ID3D12Resource *m_pVertexBuffer = NULL;
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
		ID3D12GraphicsCommandList *m_pCommandList = NULL;
	};
}