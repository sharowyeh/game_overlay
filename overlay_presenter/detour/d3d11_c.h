#pragma once
#include <d3d11.h>

#ifdef __cplusplus

// ID3D11Device c style interface duplicated from d3d11.h
typedef struct ID3D11DeviceVtbl
{
	BEGIN_INTERFACE

	HRESULT(STDMETHODCALLTYPE *QueryInterface)(
		ID3D11Device * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		_COM_Outptr_  void **ppvObject);

	ULONG(STDMETHODCALLTYPE *AddRef)(
		ID3D11Device * This);

	ULONG(STDMETHODCALLTYPE *Release)(
		ID3D11Device * This);

	HRESULT(STDMETHODCALLTYPE *CreateBuffer)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  const D3D11_BUFFER_DESC *pDesc,
		/* [annotation] */
		_In_opt_  const D3D11_SUBRESOURCE_DATA *pInitialData,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11Buffer **ppBuffer);

	HRESULT(STDMETHODCALLTYPE *CreateTexture1D)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  const D3D11_TEXTURE1D_DESC *pDesc,
		/* [annotation] */
		_In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA *pInitialData,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11Texture1D **ppTexture1D);

	HRESULT(STDMETHODCALLTYPE *CreateTexture2D)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  const D3D11_TEXTURE2D_DESC *pDesc,
		/* [annotation] */
		_In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA *pInitialData,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11Texture2D **ppTexture2D);

	HRESULT(STDMETHODCALLTYPE *CreateTexture3D)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  const D3D11_TEXTURE3D_DESC *pDesc,
		/* [annotation] */
		_In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA *pInitialData,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11Texture3D **ppTexture3D);

	HRESULT(STDMETHODCALLTYPE *CreateShaderResourceView)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  ID3D11Resource *pResource,
		/* [annotation] */
		_In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11ShaderResourceView **ppSRView);

	HRESULT(STDMETHODCALLTYPE *CreateUnorderedAccessView)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  ID3D11Resource *pResource,
		/* [annotation] */
		_In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11UnorderedAccessView **ppUAView);

	HRESULT(STDMETHODCALLTYPE *CreateRenderTargetView)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  ID3D11Resource *pResource,
		/* [annotation] */
		_In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11RenderTargetView **ppRTView);

	HRESULT(STDMETHODCALLTYPE *CreateDepthStencilView)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  ID3D11Resource *pResource,
		/* [annotation] */
		_In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11DepthStencilView **ppDepthStencilView);

	HRESULT(STDMETHODCALLTYPE *CreateInputLayout)(
		ID3D11Device * This,
		/* [annotation] */
		_In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs,
		/* [annotation] */
		_In_range_(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)  UINT NumElements,
		/* [annotation] */
		_In_reads_(BytecodeLength)  const void *pShaderBytecodeWithInputSignature,
		/* [annotation] */
		_In_  SIZE_T BytecodeLength,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11InputLayout **ppInputLayout);

	HRESULT(STDMETHODCALLTYPE *CreateVertexShader)(
		ID3D11Device * This,
		/* [annotation] */
		_In_reads_(BytecodeLength)  const void *pShaderBytecode,
		/* [annotation] */
		_In_  SIZE_T BytecodeLength,
		/* [annotation] */
		_In_opt_  ID3D11ClassLinkage *pClassLinkage,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11VertexShader **ppVertexShader);

	HRESULT(STDMETHODCALLTYPE *CreateGeometryShader)(
		ID3D11Device * This,
		/* [annotation] */
		_In_reads_(BytecodeLength)  const void *pShaderBytecode,
		/* [annotation] */
		_In_  SIZE_T BytecodeLength,
		/* [annotation] */
		_In_opt_  ID3D11ClassLinkage *pClassLinkage,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11GeometryShader **ppGeometryShader);

	HRESULT(STDMETHODCALLTYPE *CreateGeometryShaderWithStreamOutput)(
		ID3D11Device * This,
		/* [annotation] */
		_In_reads_(BytecodeLength)  const void *pShaderBytecode,
		/* [annotation] */
		_In_  SIZE_T BytecodeLength,
		/* [annotation] */
		_In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
		/* [annotation] */
		_In_range_(0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT)  UINT NumEntries,
		/* [annotation] */
		_In_reads_opt_(NumStrides)  const UINT *pBufferStrides,
		/* [annotation] */
		_In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumStrides,
		/* [annotation] */
		_In_  UINT RasterizedStream,
		/* [annotation] */
		_In_opt_  ID3D11ClassLinkage *pClassLinkage,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11GeometryShader **ppGeometryShader);

	HRESULT(STDMETHODCALLTYPE *CreatePixelShader)(
		ID3D11Device * This,
		/* [annotation] */
		_In_reads_(BytecodeLength)  const void *pShaderBytecode,
		/* [annotation] */
		_In_  SIZE_T BytecodeLength,
		/* [annotation] */
		_In_opt_  ID3D11ClassLinkage *pClassLinkage,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11PixelShader **ppPixelShader);

	HRESULT(STDMETHODCALLTYPE *CreateHullShader)(
		ID3D11Device * This,
		/* [annotation] */
		_In_reads_(BytecodeLength)  const void *pShaderBytecode,
		/* [annotation] */
		_In_  SIZE_T BytecodeLength,
		/* [annotation] */
		_In_opt_  ID3D11ClassLinkage *pClassLinkage,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11HullShader **ppHullShader);

	HRESULT(STDMETHODCALLTYPE *CreateDomainShader)(
		ID3D11Device * This,
		/* [annotation] */
		_In_reads_(BytecodeLength)  const void *pShaderBytecode,
		/* [annotation] */
		_In_  SIZE_T BytecodeLength,
		/* [annotation] */
		_In_opt_  ID3D11ClassLinkage *pClassLinkage,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11DomainShader **ppDomainShader);

	HRESULT(STDMETHODCALLTYPE *CreateComputeShader)(
		ID3D11Device * This,
		/* [annotation] */
		_In_reads_(BytecodeLength)  const void *pShaderBytecode,
		/* [annotation] */
		_In_  SIZE_T BytecodeLength,
		/* [annotation] */
		_In_opt_  ID3D11ClassLinkage *pClassLinkage,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11ComputeShader **ppComputeShader);

	HRESULT(STDMETHODCALLTYPE *CreateClassLinkage)(
		ID3D11Device * This,
		/* [annotation] */
		_COM_Outptr_  ID3D11ClassLinkage **ppLinkage);

	HRESULT(STDMETHODCALLTYPE *CreateBlendState)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  const D3D11_BLEND_DESC *pBlendStateDesc,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11BlendState **ppBlendState);

	HRESULT(STDMETHODCALLTYPE *CreateDepthStencilState)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11DepthStencilState **ppDepthStencilState);

	HRESULT(STDMETHODCALLTYPE *CreateRasterizerState)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  const D3D11_RASTERIZER_DESC *pRasterizerDesc,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11RasterizerState **ppRasterizerState);

	HRESULT(STDMETHODCALLTYPE *CreateSamplerState)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  const D3D11_SAMPLER_DESC *pSamplerDesc,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11SamplerState **ppSamplerState);

	HRESULT(STDMETHODCALLTYPE *CreateQuery)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  const D3D11_QUERY_DESC *pQueryDesc,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11Query **ppQuery);

	HRESULT(STDMETHODCALLTYPE *CreatePredicate)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  const D3D11_QUERY_DESC *pPredicateDesc,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11Predicate **ppPredicate);

	HRESULT(STDMETHODCALLTYPE *CreateCounter)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  const D3D11_COUNTER_DESC *pCounterDesc,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11Counter **ppCounter);

	HRESULT(STDMETHODCALLTYPE *CreateDeferredContext)(
		ID3D11Device * This,
		UINT ContextFlags,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11DeviceContext **ppDeferredContext);

	HRESULT(STDMETHODCALLTYPE *OpenSharedResource)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  HANDLE hResource,
		/* [annotation] */
		_In_  REFIID ReturnedInterface,
		/* [annotation] */
		_COM_Outptr_opt_  void **ppResource);

	HRESULT(STDMETHODCALLTYPE *CheckFormatSupport)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  DXGI_FORMAT Format,
		/* [annotation] */
		_Out_  UINT *pFormatSupport);

	HRESULT(STDMETHODCALLTYPE *CheckMultisampleQualityLevels)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  DXGI_FORMAT Format,
		/* [annotation] */
		_In_  UINT SampleCount,
		/* [annotation] */
		_Out_  UINT *pNumQualityLevels);

	void (STDMETHODCALLTYPE *CheckCounterInfo)(
		ID3D11Device * This,
		/* [annotation] */
		_Out_  D3D11_COUNTER_INFO *pCounterInfo);

	HRESULT(STDMETHODCALLTYPE *CheckCounter)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  const D3D11_COUNTER_DESC *pDesc,
		/* [annotation] */
		_Out_  D3D11_COUNTER_TYPE *pType,
		/* [annotation] */
		_Out_  UINT *pActiveCounters,
		/* [annotation] */
		_Out_writes_opt_(*pNameLength)  LPSTR szName,
		/* [annotation] */
		_Inout_opt_  UINT *pNameLength,
		/* [annotation] */
		_Out_writes_opt_(*pUnitsLength)  LPSTR szUnits,
		/* [annotation] */
		_Inout_opt_  UINT *pUnitsLength,
		/* [annotation] */
		_Out_writes_opt_(*pDescriptionLength)  LPSTR szDescription,
		/* [annotation] */
		_Inout_opt_  UINT *pDescriptionLength);

	HRESULT(STDMETHODCALLTYPE *CheckFeatureSupport)(
		ID3D11Device * This,
		D3D11_FEATURE Feature,
		/* [annotation] */
		_Out_writes_bytes_(FeatureSupportDataSize)  void *pFeatureSupportData,
		UINT FeatureSupportDataSize);

	HRESULT(STDMETHODCALLTYPE *GetPrivateData)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  REFGUID guid,
		/* [annotation] */
		_Inout_  UINT *pDataSize,
		/* [annotation] */
		_Out_writes_bytes_opt_(*pDataSize)  void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateData)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  REFGUID guid,
		/* [annotation] */
		_In_  UINT DataSize,
		/* [annotation] */
		_In_reads_bytes_opt_(DataSize)  const void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateDataInterface)(
		ID3D11Device * This,
		/* [annotation] */
		_In_  REFGUID guid,
		/* [annotation] */
		_In_opt_  const IUnknown *pData);

	D3D_FEATURE_LEVEL(STDMETHODCALLTYPE *GetFeatureLevel)(
		ID3D11Device * This);

	UINT(STDMETHODCALLTYPE *GetCreationFlags)(
		ID3D11Device * This);

	HRESULT(STDMETHODCALLTYPE *GetDeviceRemovedReason)(
		ID3D11Device * This);

	void (STDMETHODCALLTYPE *GetImmediateContext)(
		ID3D11Device * This,
		/* [annotation] */
		_Outptr_  ID3D11DeviceContext **ppImmediateContext);

	HRESULT(STDMETHODCALLTYPE *SetExceptionMode)(
		ID3D11Device * This,
		UINT RaiseFlags);

	UINT(STDMETHODCALLTYPE *GetExceptionMode)(
		ID3D11Device * This);

	END_INTERFACE
} ID3D11DeviceVtbl;

// ID3D11DeviceContext c style interface duplicated from d3d11.h
typedef struct ID3D11DeviceContextVtbl
{
	BEGIN_INTERFACE

	HRESULT(STDMETHODCALLTYPE *QueryInterface)(
		ID3D11DeviceContext * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		_COM_Outptr_  void **ppvObject);

	ULONG(STDMETHODCALLTYPE *AddRef)(
		ID3D11DeviceContext * This);

	ULONG(STDMETHODCALLTYPE *Release)(
		ID3D11DeviceContext * This);

	void (STDMETHODCALLTYPE *GetDevice)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_  ID3D11Device **ppDevice);

	HRESULT(STDMETHODCALLTYPE *GetPrivateData)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  REFGUID guid,
		/* [annotation] */
		_Inout_  UINT *pDataSize,
		/* [annotation] */
		_Out_writes_bytes_opt_(*pDataSize)  void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateData)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  REFGUID guid,
		/* [annotation] */
		_In_  UINT DataSize,
		/* [annotation] */
		_In_reads_bytes_opt_(DataSize)  const void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateDataInterface)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  REFGUID guid,
		/* [annotation] */
		_In_opt_  const IUnknown *pData);

	void (STDMETHODCALLTYPE *VSSetConstantBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

	void (STDMETHODCALLTYPE *PSSetShaderResources)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		_In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

	void (STDMETHODCALLTYPE *PSSetShader)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_opt_  ID3D11PixelShader *pPixelShader,
		/* [annotation] */
		_In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
		UINT NumClassInstances);

	void (STDMETHODCALLTYPE *PSSetSamplers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		_In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

	void (STDMETHODCALLTYPE *VSSetShader)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_opt_  ID3D11VertexShader *pVertexShader,
		/* [annotation] */
		_In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
		UINT NumClassInstances);

	void (STDMETHODCALLTYPE *DrawIndexed)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  UINT IndexCount,
		/* [annotation] */
		_In_  UINT StartIndexLocation,
		/* [annotation] */
		_In_  INT BaseVertexLocation);

	void (STDMETHODCALLTYPE *Draw)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  UINT VertexCount,
		/* [annotation] */
		_In_  UINT StartVertexLocation);

	HRESULT(STDMETHODCALLTYPE *Map)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Resource *pResource,
		/* [annotation] */
		_In_  UINT Subresource,
		/* [annotation] */
		_In_  D3D11_MAP MapType,
		/* [annotation] */
		_In_  UINT MapFlags,
		/* [annotation] */
		_Out_opt_  D3D11_MAPPED_SUBRESOURCE *pMappedResource);

	void (STDMETHODCALLTYPE *Unmap)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Resource *pResource,
		/* [annotation] */
		_In_  UINT Subresource);

	void (STDMETHODCALLTYPE *PSSetConstantBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

	void (STDMETHODCALLTYPE *IASetInputLayout)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_opt_  ID3D11InputLayout *pInputLayout);

	void (STDMETHODCALLTYPE *IASetVertexBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppVertexBuffers,
		/* [annotation] */
		_In_reads_opt_(NumBuffers)  const UINT *pStrides,
		/* [annotation] */
		_In_reads_opt_(NumBuffers)  const UINT *pOffsets);

	void (STDMETHODCALLTYPE *IASetIndexBuffer)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_opt_  ID3D11Buffer *pIndexBuffer,
		/* [annotation] */
		_In_  DXGI_FORMAT Format,
		/* [annotation] */
		_In_  UINT Offset);

	void (STDMETHODCALLTYPE *DrawIndexedInstanced)(
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
		_In_  UINT StartInstanceLocation);

	void (STDMETHODCALLTYPE *DrawInstanced)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  UINT VertexCountPerInstance,
		/* [annotation] */
		_In_  UINT InstanceCount,
		/* [annotation] */
		_In_  UINT StartVertexLocation,
		/* [annotation] */
		_In_  UINT StartInstanceLocation);

	void (STDMETHODCALLTYPE *GSSetConstantBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

	void (STDMETHODCALLTYPE *GSSetShader)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_opt_  ID3D11GeometryShader *pShader,
		/* [annotation] */
		_In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
		UINT NumClassInstances);

	void (STDMETHODCALLTYPE *IASetPrimitiveTopology)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  D3D11_PRIMITIVE_TOPOLOGY Topology);

	void (STDMETHODCALLTYPE *VSSetShaderResources)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		_In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

	void (STDMETHODCALLTYPE *VSSetSamplers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		_In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

	void (STDMETHODCALLTYPE *Begin)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Asynchronous *pAsync);

	void (STDMETHODCALLTYPE *End)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Asynchronous *pAsync);

	HRESULT(STDMETHODCALLTYPE *GetData)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Asynchronous *pAsync,
		/* [annotation] */
		_Out_writes_bytes_opt_(DataSize)  void *pData,
		/* [annotation] */
		_In_  UINT DataSize,
		/* [annotation] */
		_In_  UINT GetDataFlags);

	void (STDMETHODCALLTYPE *SetPredication)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_opt_  ID3D11Predicate *pPredicate,
		/* [annotation] */
		_In_  BOOL PredicateValue);

	void (STDMETHODCALLTYPE *GSSetShaderResources)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		_In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

	void (STDMETHODCALLTYPE *GSSetSamplers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		_In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

	void (STDMETHODCALLTYPE *OMSetRenderTargets)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews,
		/* [annotation] */
		_In_reads_opt_(NumViews)  ID3D11RenderTargetView *const *ppRenderTargetViews,
		/* [annotation] */
		_In_opt_  ID3D11DepthStencilView *pDepthStencilView);

	void (STDMETHODCALLTYPE *OMSetRenderTargetsAndUnorderedAccessViews)(
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
		_In_reads_opt_(NumUAVs)  const UINT *pUAVInitialCounts);

	void (STDMETHODCALLTYPE *OMSetBlendState)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_opt_  ID3D11BlendState *pBlendState,
		/* [annotation] */
		_In_opt_  const FLOAT BlendFactor[4],
		/* [annotation] */
		_In_  UINT SampleMask);

	void (STDMETHODCALLTYPE *OMSetDepthStencilState)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_opt_  ID3D11DepthStencilState *pDepthStencilState,
		/* [annotation] */
		_In_  UINT StencilRef);

	void (STDMETHODCALLTYPE *SOSetTargets)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumBuffers,
		/* [annotation] */
		_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppSOTargets,
		/* [annotation] */
		_In_reads_opt_(NumBuffers)  const UINT *pOffsets);

	void (STDMETHODCALLTYPE *DrawAuto)(
		ID3D11DeviceContext * This);

	void (STDMETHODCALLTYPE *DrawIndexedInstancedIndirect)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Buffer *pBufferForArgs,
		/* [annotation] */
		_In_  UINT AlignedByteOffsetForArgs);

	void (STDMETHODCALLTYPE *DrawInstancedIndirect)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Buffer *pBufferForArgs,
		/* [annotation] */
		_In_  UINT AlignedByteOffsetForArgs);

	void (STDMETHODCALLTYPE *Dispatch)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  UINT ThreadGroupCountX,
		/* [annotation] */
		_In_  UINT ThreadGroupCountY,
		/* [annotation] */
		_In_  UINT ThreadGroupCountZ);

	void (STDMETHODCALLTYPE *DispatchIndirect)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Buffer *pBufferForArgs,
		/* [annotation] */
		_In_  UINT AlignedByteOffsetForArgs);

	void (STDMETHODCALLTYPE *RSSetState)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_opt_  ID3D11RasterizerState *pRasterizerState);

	void (STDMETHODCALLTYPE *RSSetViewports)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
		/* [annotation] */
		_In_reads_opt_(NumViewports)  const D3D11_VIEWPORT *pViewports);

	void (STDMETHODCALLTYPE *RSSetScissorRects)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects,
		/* [annotation] */
		_In_reads_opt_(NumRects)  const D3D11_RECT *pRects);

	void (STDMETHODCALLTYPE *CopySubresourceRegion)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Resource *pDstResource,
		/* [annotation] */
		_In_  UINT DstSubresource,
		/* [annotation] */
		_In_  UINT DstX,
		/* [annotation] */
		_In_  UINT DstY,
		/* [annotation] */
		_In_  UINT DstZ,
		/* [annotation] */
		_In_  ID3D11Resource *pSrcResource,
		/* [annotation] */
		_In_  UINT SrcSubresource,
		/* [annotation] */
		_In_opt_  const D3D11_BOX *pSrcBox);

	void (STDMETHODCALLTYPE *CopyResource)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Resource *pDstResource,
		/* [annotation] */
		_In_  ID3D11Resource *pSrcResource);

	void (STDMETHODCALLTYPE *UpdateSubresource)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Resource *pDstResource,
		/* [annotation] */
		_In_  UINT DstSubresource,
		/* [annotation] */
		_In_opt_  const D3D11_BOX *pDstBox,
		/* [annotation] */
		_In_  const void *pSrcData,
		/* [annotation] */
		_In_  UINT SrcRowPitch,
		/* [annotation] */
		_In_  UINT SrcDepthPitch);

	void (STDMETHODCALLTYPE *CopyStructureCount)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Buffer *pDstBuffer,
		/* [annotation] */
		_In_  UINT DstAlignedByteOffset,
		/* [annotation] */
		_In_  ID3D11UnorderedAccessView *pSrcView);

	void (STDMETHODCALLTYPE *ClearRenderTargetView)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11RenderTargetView *pRenderTargetView,
		/* [annotation] */
		_In_  const FLOAT ColorRGBA[4]);

	void (STDMETHODCALLTYPE *ClearUnorderedAccessViewUint)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11UnorderedAccessView *pUnorderedAccessView,
		/* [annotation] */
		_In_  const UINT Values[4]);

	void (STDMETHODCALLTYPE *ClearUnorderedAccessViewFloat)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11UnorderedAccessView *pUnorderedAccessView,
		/* [annotation] */
		_In_  const FLOAT Values[4]);

	void (STDMETHODCALLTYPE *ClearDepthStencilView)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11DepthStencilView *pDepthStencilView,
		/* [annotation] */
		_In_  UINT ClearFlags,
		/* [annotation] */
		_In_  FLOAT Depth,
		/* [annotation] */
		_In_  UINT8 Stencil);

	void (STDMETHODCALLTYPE *GenerateMips)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11ShaderResourceView *pShaderResourceView);

	void (STDMETHODCALLTYPE *SetResourceMinLOD)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Resource *pResource,
		FLOAT MinLOD);

	FLOAT(STDMETHODCALLTYPE *GetResourceMinLOD)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Resource *pResource);

	void (STDMETHODCALLTYPE *ResolveSubresource)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11Resource *pDstResource,
		/* [annotation] */
		_In_  UINT DstSubresource,
		/* [annotation] */
		_In_  ID3D11Resource *pSrcResource,
		/* [annotation] */
		_In_  UINT SrcSubresource,
		/* [annotation] */
		_In_  DXGI_FORMAT Format);

	void (STDMETHODCALLTYPE *ExecuteCommandList)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_  ID3D11CommandList *pCommandList,
		BOOL RestoreContextState);

	void (STDMETHODCALLTYPE *HSSetShaderResources)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		_In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

	void (STDMETHODCALLTYPE *HSSetShader)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_opt_  ID3D11HullShader *pHullShader,
		/* [annotation] */
		_In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
		UINT NumClassInstances);

	void (STDMETHODCALLTYPE *HSSetSamplers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		_In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

	void (STDMETHODCALLTYPE *HSSetConstantBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

	void (STDMETHODCALLTYPE *DSSetShaderResources)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		_In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

	void (STDMETHODCALLTYPE *DSSetShader)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_opt_  ID3D11DomainShader *pDomainShader,
		/* [annotation] */
		_In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
		UINT NumClassInstances);

	void (STDMETHODCALLTYPE *DSSetSamplers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		_In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

	void (STDMETHODCALLTYPE *DSSetConstantBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

	void (STDMETHODCALLTYPE *CSSetShaderResources)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		_In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

	void (STDMETHODCALLTYPE *CSSetUnorderedAccessViews)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_1_UAV_SLOT_COUNT - StartSlot)  UINT NumUAVs,
		/* [annotation] */
		_In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *ppUnorderedAccessViews,
		/* [annotation] */
		_In_reads_opt_(NumUAVs)  const UINT *pUAVInitialCounts);

	void (STDMETHODCALLTYPE *CSSetShader)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_opt_  ID3D11ComputeShader *pComputeShader,
		/* [annotation] */
		_In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
		UINT NumClassInstances);

	void (STDMETHODCALLTYPE *CSSetSamplers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		_In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

	void (STDMETHODCALLTYPE *CSSetConstantBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

	void (STDMETHODCALLTYPE *VSGetConstantBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

	void (STDMETHODCALLTYPE *PSGetShaderResources)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		_Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

	void (STDMETHODCALLTYPE *PSGetShader)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_result_maybenull_  ID3D11PixelShader **ppPixelShader,
		/* [annotation] */
		_Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
		/* [annotation] */
		_Inout_opt_  UINT *pNumClassInstances);

	void (STDMETHODCALLTYPE *PSGetSamplers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		_Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

	void (STDMETHODCALLTYPE *VSGetShader)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_result_maybenull_  ID3D11VertexShader **ppVertexShader,
		/* [annotation] */
		_Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
		/* [annotation] */
		_Inout_opt_  UINT *pNumClassInstances);

	void (STDMETHODCALLTYPE *PSGetConstantBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

	void (STDMETHODCALLTYPE *IAGetInputLayout)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_result_maybenull_  ID3D11InputLayout **ppInputLayout);

	void (STDMETHODCALLTYPE *IAGetVertexBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppVertexBuffers,
		/* [annotation] */
		_Out_writes_opt_(NumBuffers)  UINT *pStrides,
		/* [annotation] */
		_Out_writes_opt_(NumBuffers)  UINT *pOffsets);

	void (STDMETHODCALLTYPE *IAGetIndexBuffer)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_opt_result_maybenull_  ID3D11Buffer **pIndexBuffer,
		/* [annotation] */
		_Out_opt_  DXGI_FORMAT *Format,
		/* [annotation] */
		_Out_opt_  UINT *Offset);

	void (STDMETHODCALLTYPE *GSGetConstantBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

	void (STDMETHODCALLTYPE *GSGetShader)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_result_maybenull_  ID3D11GeometryShader **ppGeometryShader,
		/* [annotation] */
		_Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
		/* [annotation] */
		_Inout_opt_  UINT *pNumClassInstances);

	void (STDMETHODCALLTYPE *IAGetPrimitiveTopology)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Out_  D3D11_PRIMITIVE_TOPOLOGY *pTopology);

	void (STDMETHODCALLTYPE *VSGetShaderResources)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		_Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

	void (STDMETHODCALLTYPE *VSGetSamplers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		_Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

	void (STDMETHODCALLTYPE *GetPredication)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_opt_result_maybenull_  ID3D11Predicate **ppPredicate,
		/* [annotation] */
		_Out_opt_  BOOL *pPredicateValue);

	void (STDMETHODCALLTYPE *GSGetShaderResources)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		_Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

	void (STDMETHODCALLTYPE *GSGetSamplers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		_Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

	void (STDMETHODCALLTYPE *OMGetRenderTargets)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews,
		/* [annotation] */
		_Out_writes_opt_(NumViews)  ID3D11RenderTargetView **ppRenderTargetViews,
		/* [annotation] */
		_Outptr_opt_result_maybenull_  ID3D11DepthStencilView **ppDepthStencilView);

	void (STDMETHODCALLTYPE *OMGetRenderTargetsAndUnorderedAccessViews)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumRTVs,
		/* [annotation] */
		_Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView **ppRenderTargetViews,
		/* [annotation] */
		_Outptr_opt_result_maybenull_  ID3D11DepthStencilView **ppDepthStencilView,
		/* [annotation] */
		_In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1)  UINT UAVStartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot)  UINT NumUAVs,
		/* [annotation] */
		_Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **ppUnorderedAccessViews);

	void (STDMETHODCALLTYPE *OMGetBlendState)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_opt_result_maybenull_  ID3D11BlendState **ppBlendState,
		/* [annotation] */
		_Out_opt_  FLOAT BlendFactor[4],
		/* [annotation] */
		_Out_opt_  UINT *pSampleMask);

	void (STDMETHODCALLTYPE *OMGetDepthStencilState)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_opt_result_maybenull_  ID3D11DepthStencilState **ppDepthStencilState,
		/* [annotation] */
		_Out_opt_  UINT *pStencilRef);

	void (STDMETHODCALLTYPE *SOGetTargets)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumBuffers,
		/* [annotation] */
		_Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppSOTargets);

	void (STDMETHODCALLTYPE *RSGetState)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_result_maybenull_  ID3D11RasterizerState **ppRasterizerState);

	void (STDMETHODCALLTYPE *RSGetViewports)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *pNumViewports,
		/* [annotation] */
		_Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT *pViewports);

	void (STDMETHODCALLTYPE *RSGetScissorRects)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *pNumRects,
		/* [annotation] */
		_Out_writes_opt_(*pNumRects)  D3D11_RECT *pRects);

	void (STDMETHODCALLTYPE *HSGetShaderResources)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		_Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

	void (STDMETHODCALLTYPE *HSGetShader)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_result_maybenull_  ID3D11HullShader **ppHullShader,
		/* [annotation] */
		_Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
		/* [annotation] */
		_Inout_opt_  UINT *pNumClassInstances);

	void (STDMETHODCALLTYPE *HSGetSamplers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		_Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

	void (STDMETHODCALLTYPE *HSGetConstantBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

	void (STDMETHODCALLTYPE *DSGetShaderResources)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		_Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

	void (STDMETHODCALLTYPE *DSGetShader)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_result_maybenull_  ID3D11DomainShader **ppDomainShader,
		/* [annotation] */
		_Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
		/* [annotation] */
		_Inout_opt_  UINT *pNumClassInstances);

	void (STDMETHODCALLTYPE *DSGetSamplers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		_Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

	void (STDMETHODCALLTYPE *DSGetConstantBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

	void (STDMETHODCALLTYPE *CSGetShaderResources)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
		/* [annotation] */
		_Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

	void (STDMETHODCALLTYPE *CSGetUnorderedAccessViews)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_1_UAV_SLOT_COUNT - StartSlot)  UINT NumUAVs,
		/* [annotation] */
		_Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **ppUnorderedAccessViews);

	void (STDMETHODCALLTYPE *CSGetShader)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_Outptr_result_maybenull_  ID3D11ComputeShader **ppComputeShader,
		/* [annotation] */
		_Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
		/* [annotation] */
		_Inout_opt_  UINT *pNumClassInstances);

	void (STDMETHODCALLTYPE *CSGetSamplers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
		/* [annotation] */
		_Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

	void (STDMETHODCALLTYPE *CSGetConstantBuffers)(
		ID3D11DeviceContext * This,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
		/* [annotation] */
		_In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
		/* [annotation] */
		_Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

	void (STDMETHODCALLTYPE *ClearState)(
		ID3D11DeviceContext * This);

	void (STDMETHODCALLTYPE *Flush)(
		ID3D11DeviceContext * This);

	D3D11_DEVICE_CONTEXT_TYPE(STDMETHODCALLTYPE *GetType)(
		ID3D11DeviceContext * This);

	UINT(STDMETHODCALLTYPE *GetContextFlags)(
		ID3D11DeviceContext * This);

	HRESULT(STDMETHODCALLTYPE *FinishCommandList)(
		ID3D11DeviceContext * This,
		BOOL RestoreDeferredContextState,
		/* [annotation] */
		_COM_Outptr_opt_  ID3D11CommandList **ppCommandList);

	END_INTERFACE
} ID3D11DeviceContextVtbl;

#endif