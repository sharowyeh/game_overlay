#include "d3d_detour.h"
#include "d3d12_c.h"

#ifdef LOGFILE
#undef LOGFILE
#define LOGFILE(fmt, ...) WriteLog("detour.txt", fmt, ##__VA_ARGS__)
#endif

#if _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d12.h>
#include "../../Detours/include/detours.h"

#pragma comment(lib, "d3d12.lib")
#ifdef _WIN64
#pragma comment(lib, "../Detours/lib.X64/detours.lib")
#else
#pragma comment(lib, "../Detours/lib.X86/detours.lib")
#endif

namespace detour
{
#ifdef __cplusplus
	extern "C"
	{
#endif
		ID3D12DeviceVtbl *m_pD3D12DeviceVtbl = NULL;

		HRESULT STDMETHODCALLTYPE Detour_D3D12Device_QueryInterface(
			ID3D12Device * This,
			REFIID riid,
			_COM_Outptr_  void **ppvObject)
		{

			HRESULT result = m_pD3D12DeviceVtbl->QueryInterface(This, riid, ppvObject);
			LOGFILE("%s: result=0x%x device=0x%p vobj=0x%p\n", __func__, result, This,
				(ppvObject == NULL ? NULL : (*ppvObject)));
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_D3D12Device_AddRef(
			ID3D12Device * This)
		{
			ULONG result = m_pD3D12DeviceVtbl->AddRef(This);
			LOGFILE("%s: result=0x%x device=0x%p\n", __func__, result, This);
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_D3D12Device_Release(
			ID3D12Device * This)
		{
			ULONG result = m_pD3D12DeviceVtbl->Release(This);
			LOGFILE("%s: result=0x%x device=0x%p\n", __func__, result, This);
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D12Device_CreateCommandQueue(
			ID3D12Device * This,
			_In_  const D3D12_COMMAND_QUEUE_DESC *pDesc,
			REFIID riid,
			_COM_Outptr_  void **ppCommandQueue)
		{
			HRESULT result = m_pD3D12DeviceVtbl->CreateCommandQueue(This, pDesc, riid, ppCommandQueue);
			LOGFILE("%s: result=0x%x device=0x%p queue=0x%p\n", __func__, result, This,
				(ppCommandQueue == NULL ? NULL : (*ppCommandQueue)));
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D12Device_CreateCommandAllocator(
			ID3D12Device * This,
			_In_  D3D12_COMMAND_LIST_TYPE type,
			REFIID riid,
			_COM_Outptr_  void **ppCommandAllocator)
		{
			HRESULT result = m_pD3D12DeviceVtbl->CreateCommandAllocator(This, type, riid, ppCommandAllocator);
			LOGFILE("%s: result=0x%x device=0x%p allocator=0x%p\n", __func__, result, This,
				(ppCommandAllocator == NULL ? NULL : (*ppCommandAllocator)));
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D12Device_CreateGraphicsPipelineState(
			ID3D12Device * This,
			_In_  const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc,
			REFIID riid,
			_COM_Outptr_  void **ppPipelineState)
		{
			HRESULT result = m_pD3D12DeviceVtbl->CreateGraphicsPipelineState(This, pDesc, riid, ppPipelineState);
			LOGFILE("%s: result=0x%x device=0x%p pipeline=0x%p\n", __func__, result, This,
				(ppPipelineState == NULL ? NULL : (*ppPipelineState)));
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D12Device_CreateComputePipelineState(
			ID3D12Device * This,
			_In_  const D3D12_COMPUTE_PIPELINE_STATE_DESC *pDesc,
			REFIID riid,
			_COM_Outptr_  void **ppPipelineState)
		{
			HRESULT result = m_pD3D12DeviceVtbl->CreateComputePipelineState(This, pDesc, riid, ppPipelineState);
			LOGFILE("%s: result=0x%x device=0x%p pipeline=0x%p\n", __func__, result, This,
				(ppPipelineState == NULL ? NULL : (*ppPipelineState)));
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D12Device_CreateCommandList(
			ID3D12Device * This,
			_In_  UINT nodeMask,
			_In_  D3D12_COMMAND_LIST_TYPE type,
			_In_  ID3D12CommandAllocator *pCommandAllocator,
			_In_opt_  ID3D12PipelineState *pInitialState,
			REFIID riid,
			_COM_Outptr_  void **ppCommandList)
		{
			HRESULT result = m_pD3D12DeviceVtbl->CreateCommandList(This, nodeMask, type, pCommandAllocator, pInitialState, riid, ppCommandList);
			LOGFILE("%s: result=0x%x device=0x%p allocator=0x%p cmdlist=0x%p\n", __func__, result, This, pCommandAllocator,
				(ppCommandList == NULL ? NULL : (*ppCommandList)));
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D12Device_CreateRootSignature(
			ID3D12Device * This,
			_In_  UINT nodeMask,
			_In_reads_(blobLengthInBytes)  const void *pBlobWithRootSignature,
			_In_  SIZE_T blobLengthInBytes,
			REFIID riid,
			_COM_Outptr_  void **ppvRootSignature)
		{
			HRESULT result = m_pD3D12DeviceVtbl->CreateRootSignature(This, nodeMask, pBlobWithRootSignature, blobLengthInBytes, riid, ppvRootSignature);
			LOGFILE("%s: result=0x%x device=0x%p signature=0x%p\n", __func__, result, This,
				(ppvRootSignature == NULL ? NULL : (*ppvRootSignature)));
			return result;
		}

		void  STDMETHODCALLTYPE Detour_D3D12Device_CreateShaderResourceView(
			ID3D12Device * This,
			_In_opt_  ID3D12Resource *pResource,
			_In_opt_  const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc,
			_In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor)
		{
			m_pD3D12DeviceVtbl->CreateShaderResourceView(This, pResource, pDesc, DestDescriptor);
			char desc[128] = "desc=NULL";
			if (pDesc != NULL) sprintf_s(desc, "fmt=%d dim=%d", pDesc->Format, pDesc->ViewDimension);
			LOGFILE("%s: void device=0x%p res=0x%p %s\n", __func__, This, pResource, desc);
		}

		void  STDMETHODCALLTYPE Detour_D3D12Device_CreateRenderTargetView(
			ID3D12Device * This,
			_In_opt_  ID3D12Resource *pResource,
			_In_opt_  const D3D12_RENDER_TARGET_VIEW_DESC *pDesc,
			_In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor)
		{
			m_pD3D12DeviceVtbl->CreateRenderTargetView(This, pResource, pDesc, DestDescriptor);
			char desc[128] = "desc=NULL";
			if (pDesc != NULL) sprintf_s(desc, "fmt=%d dim=%d", pDesc->Format, pDesc->ViewDimension);
			LOGFILE("%s: void device=0x%p res=0x%p %s\n", __func__, This, pResource, desc);
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D12Device_CreateCommittedResource(
			ID3D12Device * This,
			_In_  const D3D12_HEAP_PROPERTIES *pHeapProperties,
			D3D12_HEAP_FLAGS HeapFlags,
			_In_  const D3D12_RESOURCE_DESC *pDesc,
			D3D12_RESOURCE_STATES InitialResourceState,
			_In_opt_  const D3D12_CLEAR_VALUE *pOptimizedClearValue,
			REFIID riidResource,
			_COM_Outptr_opt_  void **ppvResource)
		{
			HRESULT result = m_pD3D12DeviceVtbl->CreateCommittedResource(This, pHeapProperties, HeapFlags, pDesc, InitialResourceState, pOptimizedClearValue, riidResource, ppvResource);
			LOGFILE("%s: result=0x%x device=0x%p res=0x%p\n", __func__, result, This,
				(ppvResource == NULL ? NULL : (*ppvResource)));
			return result;
		}

#ifdef __cplusplus
	}
#endif

	BOOL AttachD3D12Device(ID3D12Device *pDevice)
	{
		//TODO: may get client's command list and pipe state? for mesh rendering

		// Create list of device's virtual table address
		m_pD3D12DeviceVtbl = new ID3D12DeviceVtbl();
		// Copy virtual table addresses from referenced dll to detour attach and swap them
		memcpy(m_pD3D12DeviceVtbl, *reinterpret_cast<ID3D12DeviceVtbl **>(pDevice), sizeof(ID3D12DeviceVtbl));
		LOGFILE("%s: vtable=0x%p pid=%5d tid=%5d\n", __func__, m_pD3D12DeviceVtbl, GetCurrentProcessId(), GetCurrentThreadId());
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		HRESULT result = S_OK;
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		// Attach is usually succeeded except address is NULL or pointer of address is access violation.
		result = DetourAttach(
			&(PVOID &)m_pD3D12DeviceVtbl->QueryInterface,
			Detour_D3D12Device_QueryInterface);
		LOGFILE("%s: QueryInterface result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D12DeviceVtbl->QueryInterface, Detour_D3D12Device_QueryInterface);
		result = DetourAttach(
			&(PVOID &)m_pD3D12DeviceVtbl->AddRef,
			Detour_D3D12Device_AddRef);
		LOGFILE("%s: AddRef result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12DeviceVtbl->AddRef, Detour_D3D12Device_AddRef);
		result = DetourAttach(
			&(PVOID &)m_pD3D12DeviceVtbl->Release,
			Detour_D3D12Device_Release);
		LOGFILE("%s: Release result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12DeviceVtbl->Release, Detour_D3D12Device_Release);
		result = DetourAttach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateCommandQueue,
			Detour_D3D12Device_CreateCommandQueue);
		LOGFILE("%s: CreateCommandQueue result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12DeviceVtbl->CreateCommandQueue, Detour_D3D12Device_CreateCommandQueue);
		result = DetourAttach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateCommandAllocator,
			Detour_D3D12Device_CreateCommandAllocator);
		LOGFILE("%s: CreateCommandAllocator result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12DeviceVtbl->CreateCommandAllocator, Detour_D3D12Device_CreateCommandAllocator);
		result = DetourAttach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateGraphicsPipelineState,
			Detour_D3D12Device_CreateGraphicsPipelineState);
		LOGFILE("%s: CreateGraphicsPipelineState result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12DeviceVtbl->CreateGraphicsPipelineState, Detour_D3D12Device_CreateGraphicsPipelineState);
		result = DetourAttach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateComputePipelineState,
			Detour_D3D12Device_CreateComputePipelineState);
		LOGFILE("%s: CreateComputePipelineState result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12DeviceVtbl->CreateComputePipelineState, Detour_D3D12Device_CreateComputePipelineState);
		result = DetourAttach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateCommandList,
			Detour_D3D12Device_CreateCommandList);
		LOGFILE("%s: CreateCommandList result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12DeviceVtbl->CreateCommandList, Detour_D3D12Device_CreateCommandList);
		result = DetourAttach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateRootSignature,
			Detour_D3D12Device_CreateRootSignature);
		LOGFILE("%s: CreateRootSignature result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12DeviceVtbl->CreateRootSignature, Detour_D3D12Device_CreateRootSignature);
		result = DetourAttach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateShaderResourceView,
			Detour_D3D12Device_CreateShaderResourceView);
		LOGFILE("%s: CreateShaderResourceView result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12DeviceVtbl->CreateShaderResourceView, Detour_D3D12Device_CreateShaderResourceView);
		result = DetourAttach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateRenderTargetView,
			Detour_D3D12Device_CreateRenderTargetView);
		LOGFILE("%s: CreateRenderTargetView result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12DeviceVtbl->CreateRenderTargetView, Detour_D3D12Device_CreateRenderTargetView);
		result = DetourAttach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateCommittedResource,
			Detour_D3D12Device_CreateCommittedResource);
		LOGFILE("%s: CreateCommittedResource result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12DeviceVtbl->CreateCommittedResource, Detour_D3D12Device_CreateCommittedResource);
		// Then exception will be occurred when transaction commit.
		result = DetourTransactionCommit();
		return result == 0 ? TRUE : FALSE;
	}

	BOOL DetachD3D12Device()
	{
		if (m_pD3D12DeviceVtbl == NULL)
			return TRUE;

		long result = 0;
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());

		result = DetourDetach(
			&(PVOID &)m_pD3D12DeviceVtbl->QueryInterface,
			Detour_D3D12Device_QueryInterface);
		result = DetourDetach(
			&(PVOID &)m_pD3D12DeviceVtbl->AddRef,
			Detour_D3D12Device_AddRef);
		result = DetourDetach(
			&(PVOID &)m_pD3D12DeviceVtbl->Release,
			Detour_D3D12Device_Release);
		result = DetourDetach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateCommandQueue,
			Detour_D3D12Device_CreateCommandQueue);
		result = DetourDetach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateCommandAllocator,
			Detour_D3D12Device_CreateRenderTargetView);
		result = DetourDetach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateGraphicsPipelineState,
			Detour_D3D12Device_CreateGraphicsPipelineState);
		result = DetourDetach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateComputePipelineState,
			Detour_D3D12Device_CreateComputePipelineState);
		result = DetourDetach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateCommandList,
			Detour_D3D12Device_CreateCommandList);
		result = DetourDetach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateRootSignature,
			Detour_D3D12Device_CreateRootSignature);
		result = DetourDetach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateShaderResourceView,
			Detour_D3D12Device_CreateShaderResourceView);
		result = DetourDetach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateRenderTargetView,
			Detour_D3D12Device_CreateRenderTargetView);
		result = DetourDetach(
			&(PVOID &)m_pD3D12DeviceVtbl->CreateCommittedResource,
			Detour_D3D12Device_CreateCommittedResource);

		result = DetourTransactionCommit();
		return TRUE;
	}
}