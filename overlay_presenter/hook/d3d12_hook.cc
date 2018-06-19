#include "d3d12_hook.h"
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <dxgi1_4.h>

using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")

namespace hook
{
	HRESULT D3D12Hook::D3D12CreateResource()
	{
		HRESULT result = m_pSwapChain->GetDevice(__uuidof(ID3D12Device), reinterpret_cast<void **>(&m_pDevice));
		LOGFILE("%s: result=0x%x device=0x%p\n", __func__, result, m_pDevice);
		if (FAILED(result) || m_pDevice == NULL)
		{
			LOGFILE("%s: GetDevice failed\n", __func__);
			return result;
		}
		IDXGISwapChain3 *pSwapChain3;
		m_pSwapChain->QueryInterface(__uuidof(IDXGISwapChain3), reinterpret_cast<void **>(&pSwapChain3));
		UINT frameIndex = pSwapChain3->GetCurrentBackBufferIndex();
		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = 2;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ID3D12DescriptorHeap *m_rtvHeap;
		m_pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
		UINT m_rtvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		ID3D12Resource *m_renderTargets[2];
		// Create frame resources.
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		// Create a RTV for each frame.
		for (UINT n = 0; n < 2; n++)
		{
			m_pSwapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
			m_pDevice->CreateRenderTargetView(m_renderTargets[n], nullptr, rtvHandle);
			rtvHandle.ptr += m_rtvDescriptorSize;
		}

		return ERROR_CALL_NOT_IMPLEMENTED;
	}
}