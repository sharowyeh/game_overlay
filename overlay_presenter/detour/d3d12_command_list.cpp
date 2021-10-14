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
		ID3D12GraphicsCommandListVtbl *m_pD3D12CommandListVtbl = NULL;

		HRESULT STDMETHODCALLTYPE Detour_D3D12CommandList_QueryInterface(
			ID3D12GraphicsCommandList * This,
			REFIID riid,
			_COM_Outptr_  void **ppvObject)
		{
			HRESULT result = m_pD3D12CommandListVtbl->QueryInterface(This, riid, ppvObject);
			// too many instance created and released, prevent log with meaningless
			//LOGFILE("%s: result=0x%x device=0x%p vobj=0x%p\n", __func__, result, This,
			//	(ppvObject == NULL ? NULL : (*ppvObject)));
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_D3D12CommandList_AddRef(
			ID3D12GraphicsCommandList * This)
		{
			ULONG result = m_pD3D12CommandListVtbl->AddRef(This);
			// too many instance created and released, prevent log with meaningless
			//LOGFILE("%s: result=0x%x device=0x%p\n", __func__, result, This);
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_D3D12CommandList_Release(
			ID3D12GraphicsCommandList * This)
		{
			ULONG result = m_pD3D12CommandListVtbl->Release(This);
			// too many instance created and released, prevent log with meaningless
			//LOGFILE("%s: result=0x%x device=0x%p\n", __func__, result, This);
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D12CommandList_Close(
			ID3D12GraphicsCommandList * This)
		{
			HRESULT result = m_pD3D12CommandListVtbl->Close(This);
			LOGFILE("%s: result=0x%x device=0x%p\n", __func__, result, This);
			return result;
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D12CommandList_Reset(
			ID3D12GraphicsCommandList * This,
			_In_  ID3D12CommandAllocator *pAllocator,
			_In_opt_  ID3D12PipelineState *pInitialState)
		{
			HRESULT result = m_pD3D12CommandListVtbl->Reset(This, pAllocator, pInitialState);
			LOGFILE("%s: result=0x%x device=0x%p allocator=0x%p\n", __func__, result, This, pAllocator);
			return result;
		}

		void STDMETHODCALLTYPE Detour_D3D12CommandList_ClearState(
			ID3D12GraphicsCommandList * This,
			_In_opt_  ID3D12PipelineState *pPipelineState)
		{
			m_pD3D12CommandListVtbl->ClearState(This, pPipelineState);
			LOGFILE("%s: void device=0x%p pipeline=0x%p\n", __func__, This, pPipelineState);
		}

		void STDMETHODCALLTYPE Detour_D3D12CommandList_DrawInstanced(
			ID3D12GraphicsCommandList * This,
			_In_  UINT VertexCountPerInstance,
			_In_  UINT InstanceCount,
			_In_  UINT StartVertexLocation,
			_In_  UINT StartInstanceLocation)
		{
			m_pD3D12CommandListVtbl->DrawInstanced(This,
				VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
			LOGFILE("%s: void device=0x%p vtxcnt=%d instcnt=%d vtxloc=0x%x instloc=0x%x\n", __func__, This,
				VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
		}

		void STDMETHODCALLTYPE Detour_D3D12CommandList_DrawIndexedInstanced(
			ID3D12GraphicsCommandList * This,
			_In_  UINT IndexCountPerInstance,
			_In_  UINT InstanceCount,
			_In_  UINT StartIndexLocation,
			_In_  INT BaseVertexLocation,
			_In_  UINT StartInstanceLocation)
		{
			m_pD3D12CommandListVtbl->DrawIndexedInstanced(This,
				IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
			LOGFILE("%s: void device=0x%p idxcnt=%d instcnt=%d idxloc=0x%x vtxloc=0x%x instloc=0x%x\n", __func__,
				This, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
		}

		//TODO: more APIs for OnRender()

#ifdef __cplusplus
	}
#endif

	BOOL AttachD3D12CommandList(ID3D12GraphicsCommandList *pDevice)
	{
		//TODO: may get client's command list and pipe state? for mesh rendering

		// Create list of device's virtual table address
		m_pD3D12CommandListVtbl = new ID3D12GraphicsCommandListVtbl();
		// Copy virtual table addresses from referenced dll to detour attach and swap them
		memcpy(m_pD3D12CommandListVtbl, *reinterpret_cast<ID3D12GraphicsCommandListVtbl **>(pDevice), sizeof(ID3D12GraphicsCommandListVtbl));
		LOGFILE("%s: vtable=0x%p pid=%5d tid=%5d\n", __func__, m_pD3D12CommandListVtbl, GetCurrentProcessId(), GetCurrentThreadId());
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		HRESULT result = S_OK;
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		// Attach is usually succeeded except address is NULL or pointer of address is access violation.
		result = DetourAttach(
			&(PVOID &)m_pD3D12CommandListVtbl->QueryInterface,
			Detour_D3D12CommandList_QueryInterface);
		LOGFILE("%s: QueryInterface result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D12CommandListVtbl->QueryInterface, Detour_D3D12CommandList_QueryInterface);
		result = DetourAttach(
			&(PVOID &)m_pD3D12CommandListVtbl->AddRef,
			Detour_D3D12CommandList_AddRef);
		LOGFILE("%s: AddRef result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12CommandListVtbl->AddRef, Detour_D3D12CommandList_AddRef);
		result = DetourAttach(
			&(PVOID &)m_pD3D12CommandListVtbl->Release,
			Detour_D3D12CommandList_Release);
		LOGFILE("%s: Release result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12CommandListVtbl->Release, Detour_D3D12CommandList_Release);
		result = DetourAttach(
			&(PVOID &)m_pD3D12CommandListVtbl->Close,
			Detour_D3D12CommandList_Close);
		LOGFILE("%s: Close result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12CommandListVtbl->Close, Detour_D3D12CommandList_Close);
		result = DetourAttach(
			&(PVOID &)m_pD3D12CommandListVtbl->Reset,
			Detour_D3D12CommandList_Reset);
		LOGFILE("%s: Reset result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12CommandListVtbl->Reset, Detour_D3D12CommandList_Reset);
		result = DetourAttach(
			&(PVOID &)m_pD3D12CommandListVtbl->ClearState,
			Detour_D3D12CommandList_ClearState);
		LOGFILE("%s: ClearState result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12CommandListVtbl->ClearState, Detour_D3D12CommandList_ClearState);
		result = DetourAttach(
			&(PVOID &)m_pD3D12CommandListVtbl->DrawInstanced,
			Detour_D3D12CommandList_DrawInstanced);
		LOGFILE("%s: DrawInstanced result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12CommandListVtbl->DrawInstanced, Detour_D3D12CommandList_DrawInstanced);
		result = DetourAttach(
			&(PVOID &)m_pD3D12CommandListVtbl->DrawIndexedInstanced,
			Detour_D3D12CommandList_DrawIndexedInstanced);
		LOGFILE("%s: CreateCommandList result=0x%x 0x%p -> 0x%p\n", __func__, result,
			m_pD3D12CommandListVtbl->DrawIndexedInstanced, Detour_D3D12CommandList_DrawIndexedInstanced);
		// Then exception will be occurred when transaction commit.
		result = DetourTransactionCommit();
		return result == 0 ? TRUE : FALSE;
	}

	BOOL DetachD3D12CommandList()
	{
		if (m_pD3D12CommandListVtbl == NULL)
			return TRUE;

		long result = 0;
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());

		result = DetourDetach(
			&(PVOID &)m_pD3D12CommandListVtbl->QueryInterface,
			Detour_D3D12CommandList_QueryInterface);
		result = DetourDetach(
			&(PVOID &)m_pD3D12CommandListVtbl->AddRef,
			Detour_D3D12CommandList_AddRef);
		result = DetourDetach(
			&(PVOID &)m_pD3D12CommandListVtbl->Release,
			Detour_D3D12CommandList_Release);
		result = DetourDetach(
			&(PVOID &)m_pD3D12CommandListVtbl->Close,
			Detour_D3D12CommandList_Close);
		result = DetourDetach(
			&(PVOID &)m_pD3D12CommandListVtbl->Reset,
			Detour_D3D12CommandList_Reset);
		result = DetourDetach(
			&(PVOID &)m_pD3D12CommandListVtbl->ClearState,
			Detour_D3D12CommandList_ClearState);
		result = DetourDetach(
			&(PVOID &)m_pD3D12CommandListVtbl->DrawInstanced,
			Detour_D3D12CommandList_DrawInstanced);
		result = DetourDetach(
			&(PVOID &)m_pD3D12CommandListVtbl->DrawIndexedInstanced,
			Detour_D3D12CommandList_DrawIndexedInstanced);

		result = DetourTransactionCommit();
		return TRUE;
	}
}