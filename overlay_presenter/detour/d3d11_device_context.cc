#include "d3d11_detour.h"
#include "d3d11_c.h"

#ifdef LOGFILE
#undef LOGFILE
#define LOGFILE(fmt, ...) WriteLog("detour.txt", fmt, ##__VA_ARGS__)
#endif

#if _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <dxgi.h>
#include "../../Detours/include/detours.h"

#pragma comment(lib, "d3d11.lib")
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
		ID3D11DeviceContextVtbl *m_pD3D11DeviceContextVtbl = NULL;

		HRESULT STDMETHODCALLTYPE Detour_D3D11DeviceContext_QueryInterface(
			ID3D11DeviceContext * This,
			/* [in] */ REFIID riid,
			/* [annotation][iid_is][out] */
			_COM_Outptr_  void **ppvObject)
		{
			HRESULT result = m_pD3D11DeviceContextVtbl->QueryInterface(This, riid, ppvObject);
			LOGFILE("%s: result=0x%x context=0x%p vobj=0x%p\n", __func__, result, This, (*ppvObject));
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_D3D11DeviceContext_AddRef(
			ID3D11DeviceContext * This)
		{
			ULONG result = m_pD3D11DeviceContextVtbl->AddRef(This);
			LOGFILE("%s: result=0x%x context=0x%p\n", __func__, result, This);
			return result;
		}

		ULONG STDMETHODCALLTYPE Detour_D3D11DeviceContext_Release(
			ID3D11DeviceContext * This)
		{
			ULONG result = m_pD3D11DeviceContextVtbl->Release(This);
			LOGFILE("%s: result=0x%x context=0x%p\n", __func__, result, This);
			return result;
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_DrawIndexed(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_  UINT IndexCount,
			/* [annotation] */
			_In_  UINT StartIndexLocation,
			/* [annotation] */
			_In_  INT BaseVertexLocation)
		{
			m_pD3D11DeviceContextVtbl->DrawIndexed(This, IndexCount, StartIndexLocation, BaseVertexLocation);
			LOGFILE("%s: context=0x%p index_count=%d start_index=%d base_vertex=%d\n", __func__, This, IndexCount, StartIndexLocation, BaseVertexLocation);
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_Draw(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_  UINT VertexCount,
			/* [annotation] */
			_In_  UINT StartVertexLocation)
		{
			m_pD3D11DeviceContextVtbl->Draw(This, VertexCount, StartVertexLocation);
			LOGFILE("%s: context=0x%p vertex_count=%d start_vertex=%d\n", __func__, This, VertexCount, StartVertexLocation);
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_DrawIndexedInstanced(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_  UINT IndexCountPerInstance,
			/* [annotation] */
			_In_  UINT InstanceCount,
			/* [annotation] */
			_In_  UINT StartIndexLocation,
			/* [annotation] */
			_In_  INT BaseVertexLocation,
			/* [annotation] */
			_In_  UINT StartInstanceLocation)
		{
			m_pD3D11DeviceContextVtbl->DrawIndexedInstanced(This, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
			LOGFILE("%s: context=0x%p index_count=%d instance_count=%d start_index=%d base_vertex=%d start_instance=%d\n", __func__, This, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_DrawInstanced(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_  UINT VertexCountPerInstance,
			/* [annotation] */
			_In_  UINT InstanceCount,
			/* [annotation] */
			_In_  UINT StartVertexLocation,
			/* [annotation] */
			_In_  UINT StartInstanceLocation)
		{
			m_pD3D11DeviceContextVtbl->DrawInstanced(This, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
			LOGFILE("%s: context=0x%p vertex_count=%d instance_count=%d start_vertex=%d start_instance=%d\n", __func__, This, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_Begin(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_  ID3D11Asynchronous *pAsync)
		{
			m_pD3D11DeviceContextVtbl->Begin(This, pAsync);
			LOGFILE("%s: context=0x%p async=0x%p size=%d\n", __func__, This, pAsync, pAsync->GetDataSize());
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_End(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_  ID3D11Asynchronous *pAsync)
		{
			m_pD3D11DeviceContextVtbl->End(This, pAsync);
			LOGFILE("%s: context=0x%p async=0x%p size=%d\n", __func__, This, pAsync, pAsync->GetDataSize());
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_OMSetRenderTargets(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews,
			/* [annotation] */
			_In_reads_opt_(NumViews)  ID3D11RenderTargetView *const *ppRenderTargetViews,
			/* [annotation] */
			_In_opt_  ID3D11DepthStencilView *pDepthStencilView)
		{
			m_pD3D11DeviceContextVtbl->OMSetRenderTargets(This, NumViews, ppRenderTargetViews, pDepthStencilView);
			LOGFILE("%s: context=0x%p rtvs=%d rtv0=0x%p depth=0x%p\n", __func__, This, NumViews,
				(ppRenderTargetViews == NULL) ? NULL : ppRenderTargetViews[0],
				(pDepthStencilView == NULL) ? NULL : pDepthStencilView);
			//TODO: if(ppRenderTargetViews[0] != NULL) ppRenderTargetViews[0]->GetDesc(&rtvDesc)
			//TODO: if(pDepthStencilView != NULL) pDepthStencilView->GetDesc(&sdvDesc)
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_  UINT NumRTVs,
			/* [annotation] */
			_In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const *ppRenderTargetViews,
			/* [annotation] */
			_In_opt_  ID3D11DepthStencilView *pDepthStencilView,
			/* [annotation] */
			_In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1)  UINT UAVStartSlot,
			/* [annotation] */
			_In_  UINT NumUAVs,
			/* [annotation] */
			_In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *ppUnorderedAccessViews,
			/* [annotation] */
			_In_reads_opt_(NumUAVs)  const UINT *pUAVInitialCounts)
		{
			m_pD3D11DeviceContextVtbl->OMSetRenderTargetsAndUnorderedAccessViews(This, NumRTVs, ppRenderTargetViews,
				pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
			//TODO: function maybe override of OMSetRenderTargets that can be skipped log(we will only detour highest level funcion)
			LOGFILE("%s: context=0x%p rtvs=%d rtv0=0x%p depth=0x%p\n", __func__, This, NumRTVs,
				(ppRenderTargetViews == NULL) ? NULL : ppRenderTargetViews[0],
				(pDepthStencilView == NULL) ? NULL : pDepthStencilView);
			//TODO: if(ppRenderTargetViews[0] != NULL) ppRenderTargetViews[0]->GetDesc(&rtvDesc)
			//TODO: if(pDepthStencilView != NULL) pDepthStencilView->GetDesc(&sdvDesc)
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_OMSetBlendState(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_opt_  ID3D11BlendState *pBlendState,
			/* [annotation] */
			_In_opt_  const FLOAT BlendFactor[4],
			/* [annotation] */
			_In_  UINT SampleMask)
		{
			m_pD3D11DeviceContextVtbl->OMSetBlendState(This, pBlendState, BlendFactor, SampleMask);
			char factor[128] = "NULL";
			if (BlendFactor != NULL) sprintf_s(factor, "%f,%f,%f,%f", BlendFactor[0], BlendFactor[1], BlendFactor[2], BlendFactor[3]);
			LOGFILE("%s: context=0x%p state=0x%p factor=%s mask=%x\n", __func__, This,
				(pBlendState == NULL) ? NULL : pBlendState,
				factor, SampleMask);
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_DrawAuto(
			ID3D11DeviceContext * This)
		{
			m_pD3D11DeviceContextVtbl->DrawAuto(This);
			LOGFILE("%s: context=0x%p\n", __func__, This);
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_DrawIndexedInstancedIndirect(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_  ID3D11Buffer *pBufferForArgs,
			/* [annotation] */
			_In_  UINT AlignedByteOffsetForArgs)
		{
			m_pD3D11DeviceContextVtbl->DrawIndexedInstancedIndirect(This, pBufferForArgs, AlignedByteOffsetForArgs);
			LOGFILE("%s: context=0x%p buff=0x%p align=%d\n", __func__, This, pBufferForArgs, AlignedByteOffsetForArgs);
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_DrawInstancedIndirect(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_  ID3D11Buffer *pBufferForArgs,
			/* [annotation] */
			_In_  UINT AlignedByteOffsetForArgs)
		{
			m_pD3D11DeviceContextVtbl->DrawInstancedIndirect(This, pBufferForArgs, AlignedByteOffsetForArgs);
			LOGFILE("%s: context=0x%p buff=0x%p align=%d\n", __func__, This, pBufferForArgs, AlignedByteOffsetForArgs);
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_RSSetState(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_opt_  ID3D11RasterizerState *pRasterizerState)
		{
			m_pD3D11DeviceContextVtbl->RSSetState(This, pRasterizerState);
			LOGFILE("%s: context=0x%p state=0x%p\n", __func__, This,
				(pRasterizerState == NULL) ? NULL : pRasterizerState);
				//TODO: pRasterizerState->GetDesc(&rasDesc)
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_RSSetViewports(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
			/* [annotation] */
			_In_reads_opt_(NumViewports)  const D3D11_VIEWPORT *pViewports)
		{
			m_pD3D11DeviceContextVtbl->RSSetViewports(This, NumViewports, pViewports);
			char vp[128] = "NULL";
			if (pViewports != NULL) sprintf_s(vp, "%f,%f,%f,%f", pViewports[0].TopLeftX, pViewports[0].TopLeftY, pViewports[0].Width, pViewports[0].Height);
			LOGFILE("%s: context=0x%p vps=%d vp0=%s\n", __func__, This, NumViewports, vp);
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_ClearRenderTargetView(
			ID3D11DeviceContext * This,
			/* [annotation] */
			_In_  ID3D11RenderTargetView *pRenderTargetView,
			/* [annotation] */
			_In_  const FLOAT ColorRGBA[4])
		{
			m_pD3D11DeviceContextVtbl->ClearRenderTargetView(This, pRenderTargetView, ColorRGBA);
			D3D11_RENDER_TARGET_VIEW_DESC desc = {};
			pRenderTargetView->GetDesc(&desc);
			LOGFILE("%s: context=0x%p rtv=0x%p fmt=%d dim=%d color=%f,%f,%f,%f\n", __func__, This, pRenderTargetView,
				desc.Format, desc.ViewDimension, ColorRGBA[0], ColorRGBA[1], ColorRGBA[2], ColorRGBA[3]);
		}

		void STDMETHODCALLTYPE Detour_D3D11DeviceContext_ClearState(
			ID3D11DeviceContext * This)
		{
			m_pD3D11DeviceContextVtbl->ClearState(This);
			LOGFILE("%s: context=0x%p\n", __func__, This);
		}

		HRESULT STDMETHODCALLTYPE Detour_D3D11DeviceContext_FinishCommandList(
			ID3D11DeviceContext * This,
			BOOL RestoreDeferredContextState,
			/* [annotation] */
			_COM_Outptr_opt_  ID3D11CommandList **ppCommandList)
		{
			HRESULT result = m_pD3D11DeviceContextVtbl->FinishCommandList(This, RestoreDeferredContextState, ppCommandList);
			LOGFILE("%s: result=0x%x context=0x%p restore=%d cmds=0x%p\n", __func__, result, This, RestoreDeferredContextState,
				(ppCommandList == NULL) ? NULL : (*ppCommandList));
			return result;
		}

#ifdef __cplusplus
	}
#endif

	BOOL AttachD3D11DeviceContext(ID3D11DeviceContext *pDeviceContext)
	{
		// Create a list of virtual table address
		m_pD3D11DeviceContextVtbl = new ID3D11DeviceContextVtbl();
		// Copy virtual table addresses from referenced dll which loaded from current process
		memcpy(m_pD3D11DeviceContextVtbl, *reinterpret_cast<ID3D11DeviceContextVtbl **>(pDeviceContext), sizeof(ID3D11DeviceContextVtbl));
		LOGFILE("%s: vtable=0x%p pid=%5d tid=%5d\n", __func__, m_pD3D11DeviceContextVtbl, GetCurrentProcessId(), GetCurrentThreadId());
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		HRESULT result = S_OK;
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		// Attach is usually succeeded except address is NULL or pointer of address is access violation.
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->QueryInterface,
			Detour_D3D11DeviceContext_QueryInterface);
		LOGFILE("%s: AddRef result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->QueryInterface, Detour_D3D11DeviceContext_QueryInterface);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->AddRef,
			Detour_D3D11DeviceContext_AddRef);
		LOGFILE("%s: AddRef result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->AddRef, Detour_D3D11DeviceContext_AddRef);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->Release,
			Detour_D3D11DeviceContext_Release);
		LOGFILE("%s: Release result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->Release, Detour_D3D11DeviceContext_Release);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->DrawIndexed,
			Detour_D3D11DeviceContext_DrawIndexed);
		LOGFILE("%s: DrawIndexed result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->DrawIndexed, Detour_D3D11DeviceContext_DrawIndexed);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->Draw,
			Detour_D3D11DeviceContext_Draw);
		LOGFILE("%s: Draw result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->Draw, Detour_D3D11DeviceContext_Draw);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->DrawIndexedInstanced,
			Detour_D3D11DeviceContext_DrawIndexedInstanced);
		LOGFILE("%s: DrawIndexedInstanced result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->DrawIndexedInstanced, Detour_D3D11DeviceContext_DrawIndexedInstanced);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->DrawInstanced,
			Detour_D3D11DeviceContext_DrawInstanced);
		LOGFILE("%s: DrawInstanced result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->DrawInstanced, Detour_D3D11DeviceContext_DrawInstanced);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->Begin,
			Detour_D3D11DeviceContext_Begin);
		LOGFILE("%s: Begin result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->Begin, Detour_D3D11DeviceContext_Begin);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->End,
			Detour_D3D11DeviceContext_End);
		LOGFILE("%s: End result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->End, Detour_D3D11DeviceContext_End);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->OMSetRenderTargets,
			Detour_D3D11DeviceContext_OMSetRenderTargets);
		LOGFILE("%s: OMSetRenderTargets result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->OMSetRenderTargets, Detour_D3D11DeviceContext_OMSetRenderTargets);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->OMSetRenderTargetsAndUnorderedAccessViews,
			Detour_D3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews);
		LOGFILE("%s: OMSetRenderTargetsAndUnorderedAccessViews result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->OMSetRenderTargetsAndUnorderedAccessViews, Detour_D3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->OMSetBlendState,
			Detour_D3D11DeviceContext_OMSetBlendState);
		LOGFILE("%s: OMSetBlendState result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->OMSetBlendState, Detour_D3D11DeviceContext_OMSetBlendState);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->DrawAuto,
			Detour_D3D11DeviceContext_DrawAuto);
		LOGFILE("%s: DrawAuto result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->DrawAuto, Detour_D3D11DeviceContext_DrawAuto);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->DrawIndexedInstancedIndirect,
			Detour_D3D11DeviceContext_DrawIndexedInstancedIndirect);
		LOGFILE("%s: DrawIndexedInstancedIndirect result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->DrawIndexedInstancedIndirect, Detour_D3D11DeviceContext_DrawIndexedInstancedIndirect);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->DrawInstancedIndirect,
			Detour_D3D11DeviceContext_DrawInstancedIndirect);
		LOGFILE("%s: DrawInstancedIndirect result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->DrawInstancedIndirect, Detour_D3D11DeviceContext_DrawInstancedIndirect);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->RSSetState,
			Detour_D3D11DeviceContext_RSSetState);
		LOGFILE("%s: RSSetState result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->RSSetState, Detour_D3D11DeviceContext_RSSetState);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->RSSetViewports,
			Detour_D3D11DeviceContext_RSSetViewports);
		LOGFILE("%s: RSSetViewports result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->RSSetViewports, Detour_D3D11DeviceContext_RSSetViewports);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->ClearRenderTargetView,
			Detour_D3D11DeviceContext_ClearRenderTargetView);
		LOGFILE("%s: ClearRenderTargetView result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->ClearRenderTargetView, Detour_D3D11DeviceContext_ClearRenderTargetView);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->ClearState,
			Detour_D3D11DeviceContext_ClearState);
		LOGFILE("%s: ClearState result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->ClearState, Detour_D3D11DeviceContext_ClearState);
		result = DetourAttach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->FinishCommandList,
			Detour_D3D11DeviceContext_FinishCommandList);
		LOGFILE("%s: FinishCommandList result=0x%x 0x%p->0x%p\n", __func__, result,
			m_pD3D11DeviceContextVtbl->FinishCommandList, Detour_D3D11DeviceContext_FinishCommandList);
		// Then exception will be occurred when transaction commit with invalid address.
		result = DetourTransactionCommit();
		return result == 0 ? TRUE : FALSE;
	}

	BOOL DetachD3D11DeviceContext()
	{
		if (m_pD3D11DeviceContextVtbl == NULL)
			return TRUE;

		long result = 0;
		DisableThreadLibraryCalls(GetModuleHandle(NULL));
		result = DetourTransactionBegin();
		result = DetourUpdateThread(GetCurrentThread());
		//TODO: check m_pD3D11DeviceContextVtbl->method address is the same with previous given pDeviceContext?
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->QueryInterface,
			Detour_D3D11DeviceContext_QueryInterface);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->AddRef,
			Detour_D3D11DeviceContext_AddRef);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->Release,
			Detour_D3D11DeviceContext_Release);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->DrawIndexed,
			Detour_D3D11DeviceContext_DrawIndexed);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->Draw,
			Detour_D3D11DeviceContext_Draw);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->DrawIndexedInstanced,
			Detour_D3D11DeviceContext_DrawIndexedInstanced);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->DrawInstanced,
			Detour_D3D11DeviceContext_DrawInstanced);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->Begin,
			Detour_D3D11DeviceContext_Begin);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->End,
			Detour_D3D11DeviceContext_End);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->OMSetRenderTargets,
			Detour_D3D11DeviceContext_OMSetRenderTargets);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->OMSetRenderTargetsAndUnorderedAccessViews,
			Detour_D3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->OMSetBlendState,
			Detour_D3D11DeviceContext_OMSetBlendState);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->DrawAuto,
			Detour_D3D11DeviceContext_DrawAuto);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->DrawIndexedInstancedIndirect,
			Detour_D3D11DeviceContext_DrawIndexedInstancedIndirect);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->DrawInstancedIndirect,
			Detour_D3D11DeviceContext_DrawInstancedIndirect);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->RSSetState,
			Detour_D3D11DeviceContext_RSSetState);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->RSSetViewports,
			Detour_D3D11DeviceContext_RSSetViewports);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->ClearRenderTargetView,
			Detour_D3D11DeviceContext_ClearRenderTargetView);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->ClearState,
			Detour_D3D11DeviceContext_ClearState);
		result = DetourDetach(
			&(PVOID &)m_pD3D11DeviceContextVtbl->FinishCommandList,
			Detour_D3D11DeviceContext_FinishCommandList);

		result = DetourTransactionCommit();
		return TRUE;
	}
}