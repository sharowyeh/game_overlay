#pragma once
#include <d3d12.h>

#ifdef __cplusplus

// ID3D12Device c style interface duplicated from d3d12.h
typedef struct ID3D12DeviceVtbl
{
	BEGIN_INTERFACE
		
	HRESULT(STDMETHODCALLTYPE *QueryInterface)(
		ID3D12Device * This,
		REFIID riid,
		_COM_Outptr_  void **ppvObject);

	ULONG(STDMETHODCALLTYPE *AddRef)(
		ID3D12Device * This);

	ULONG(STDMETHODCALLTYPE *Release)(
		ID3D12Device * This);

	HRESULT(STDMETHODCALLTYPE *GetPrivateData)(
		ID3D12Device * This,
		_In_  REFGUID guid,
		_Inout_  UINT *pDataSize,
		_Out_writes_bytes_opt_(*pDataSize)  void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateData)(
		ID3D12Device * This,
		_In_  REFGUID guid,
		_In_  UINT DataSize,
		_In_reads_bytes_opt_(DataSize)  const void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateDataInterface)(
		ID3D12Device * This,
		_In_  REFGUID guid,
		_In_opt_  const IUnknown *pData);

	HRESULT(STDMETHODCALLTYPE *SetName)(
		ID3D12Device * This,
		_In_z_  LPCWSTR Name);

	UINT(STDMETHODCALLTYPE *GetNodeCount)(
		ID3D12Device * This);

	HRESULT(STDMETHODCALLTYPE *CreateCommandQueue)(
		ID3D12Device * This,
		_In_  const D3D12_COMMAND_QUEUE_DESC *pDesc,
		REFIID riid,
		_COM_Outptr_  void **ppCommandQueue);

	HRESULT(STDMETHODCALLTYPE *CreateCommandAllocator)(
		ID3D12Device * This,
		_In_  D3D12_COMMAND_LIST_TYPE type,
		REFIID riid,
		_COM_Outptr_  void **ppCommandAllocator);

	HRESULT(STDMETHODCALLTYPE *CreateGraphicsPipelineState)(
		ID3D12Device * This,
		_In_  const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc,
		REFIID riid,
		_COM_Outptr_  void **ppPipelineState);

	HRESULT(STDMETHODCALLTYPE *CreateComputePipelineState)(
		ID3D12Device * This,
		_In_  const D3D12_COMPUTE_PIPELINE_STATE_DESC *pDesc,
		REFIID riid,
		_COM_Outptr_  void **ppPipelineState);

	HRESULT(STDMETHODCALLTYPE *CreateCommandList)(
		ID3D12Device * This,
		_In_  UINT nodeMask,
		_In_  D3D12_COMMAND_LIST_TYPE type,
		_In_  ID3D12CommandAllocator *pCommandAllocator,
		_In_opt_  ID3D12PipelineState *pInitialState,
		REFIID riid,
		_COM_Outptr_  void **ppCommandList);

	HRESULT(STDMETHODCALLTYPE *CheckFeatureSupport)(
		ID3D12Device * This,
		D3D12_FEATURE Feature,
		_Inout_updates_bytes_(FeatureSupportDataSize)  void *pFeatureSupportData,
		UINT FeatureSupportDataSize);

	HRESULT(STDMETHODCALLTYPE *CreateDescriptorHeap)(
		ID3D12Device * This,
		_In_  const D3D12_DESCRIPTOR_HEAP_DESC *pDescriptorHeapDesc,
		REFIID riid,
		_COM_Outptr_  void **ppvHeap);

	UINT(STDMETHODCALLTYPE *GetDescriptorHandleIncrementSize)(
		ID3D12Device * This,
		_In_  D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType);

	HRESULT(STDMETHODCALLTYPE *CreateRootSignature)(
		ID3D12Device * This,
		_In_  UINT nodeMask,
		_In_reads_(blobLengthInBytes)  const void *pBlobWithRootSignature,
		_In_  SIZE_T blobLengthInBytes,
		REFIID riid,
		_COM_Outptr_  void **ppvRootSignature);

	void (STDMETHODCALLTYPE *CreateConstantBufferView)(
		ID3D12Device * This,
		_In_opt_  const D3D12_CONSTANT_BUFFER_VIEW_DESC *pDesc,
		_In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

	void (STDMETHODCALLTYPE *CreateShaderResourceView)(
		ID3D12Device * This,
		_In_opt_  ID3D12Resource *pResource,
		_In_opt_  const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc,
		_In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

	void (STDMETHODCALLTYPE *CreateUnorderedAccessView)(
		ID3D12Device * This,
		_In_opt_  ID3D12Resource *pResource,
		_In_opt_  ID3D12Resource *pCounterResource,
		_In_opt_  const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc,
		_In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

	void (STDMETHODCALLTYPE *CreateRenderTargetView)(
		ID3D12Device * This,
		_In_opt_  ID3D12Resource *pResource,
		_In_opt_  const D3D12_RENDER_TARGET_VIEW_DESC *pDesc,
		_In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

	void (STDMETHODCALLTYPE *CreateDepthStencilView)(
		ID3D12Device * This,
		_In_opt_  ID3D12Resource *pResource,
		_In_opt_  const D3D12_DEPTH_STENCIL_VIEW_DESC *pDesc,
		_In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

	void (STDMETHODCALLTYPE *CreateSampler)(
		ID3D12Device * This,
		_In_  const D3D12_SAMPLER_DESC *pDesc,
		_In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

	void (STDMETHODCALLTYPE *CopyDescriptors)(
		ID3D12Device * This,
		_In_  UINT NumDestDescriptorRanges,
		_In_reads_(NumDestDescriptorRanges)  const D3D12_CPU_DESCRIPTOR_HANDLE *pDestDescriptorRangeStarts,
		_In_reads_opt_(NumDestDescriptorRanges)  const UINT *pDestDescriptorRangeSizes,
		_In_  UINT NumSrcDescriptorRanges,
		_In_reads_(NumSrcDescriptorRanges)  const D3D12_CPU_DESCRIPTOR_HANDLE *pSrcDescriptorRangeStarts,
		_In_reads_opt_(NumSrcDescriptorRanges)  const UINT *pSrcDescriptorRangeSizes,
		_In_  D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType);

	void (STDMETHODCALLTYPE *CopyDescriptorsSimple)(
		ID3D12Device * This,
		_In_  UINT NumDescriptors,
		_In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptorRangeStart,
		_In_  D3D12_CPU_DESCRIPTOR_HANDLE SrcDescriptorRangeStart,
		_In_  D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType);

	D3D12_RESOURCE_ALLOCATION_INFO(STDMETHODCALLTYPE *GetResourceAllocationInfo)(
		ID3D12Device * This,
		_In_  UINT visibleMask,
		_In_  UINT numResourceDescs,
		_In_reads_(numResourceDescs)  const D3D12_RESOURCE_DESC *pResourceDescs);

	D3D12_HEAP_PROPERTIES(STDMETHODCALLTYPE *GetCustomHeapProperties)(
		ID3D12Device * This,
		_In_  UINT nodeMask,
		D3D12_HEAP_TYPE heapType);

	HRESULT(STDMETHODCALLTYPE *CreateCommittedResource)(
		ID3D12Device * This,
		_In_  const D3D12_HEAP_PROPERTIES *pHeapProperties,
		D3D12_HEAP_FLAGS HeapFlags,
		_In_  const D3D12_RESOURCE_DESC *pDesc,
		D3D12_RESOURCE_STATES InitialResourceState,
		_In_opt_  const D3D12_CLEAR_VALUE *pOptimizedClearValue,
		REFIID riidResource,
		_COM_Outptr_opt_  void **ppvResource);

	HRESULT(STDMETHODCALLTYPE *CreateHeap)(
		ID3D12Device * This,
		_In_  const D3D12_HEAP_DESC *pDesc,
		REFIID riid,
		_COM_Outptr_opt_  void **ppvHeap);

	HRESULT(STDMETHODCALLTYPE *CreatePlacedResource)(
		ID3D12Device * This,
		_In_  ID3D12Heap *pHeap,
		UINT64 HeapOffset,
		_In_  const D3D12_RESOURCE_DESC *pDesc,
		D3D12_RESOURCE_STATES InitialState,
		_In_opt_  const D3D12_CLEAR_VALUE *pOptimizedClearValue,
		REFIID riid,
		_COM_Outptr_opt_  void **ppvResource);

	HRESULT(STDMETHODCALLTYPE *CreateReservedResource)(
		ID3D12Device * This,
		_In_  const D3D12_RESOURCE_DESC *pDesc,
		D3D12_RESOURCE_STATES InitialState,
		_In_opt_  const D3D12_CLEAR_VALUE *pOptimizedClearValue,
		REFIID riid,
		_COM_Outptr_opt_  void **ppvResource);

	HRESULT(STDMETHODCALLTYPE *CreateSharedHandle)(
		ID3D12Device * This,
		_In_  ID3D12DeviceChild *pObject,
		_In_opt_  const SECURITY_ATTRIBUTES *pAttributes,
		DWORD Access,
		_In_opt_  LPCWSTR Name,
		_Out_  HANDLE *pHandle);

	HRESULT(STDMETHODCALLTYPE *OpenSharedHandle)(
		ID3D12Device * This,
		_In_  HANDLE NTHandle,
		REFIID riid,
		_COM_Outptr_opt_  void **ppvObj);

	HRESULT(STDMETHODCALLTYPE *OpenSharedHandleByName)(
		ID3D12Device * This,
		_In_  LPCWSTR Name,
		DWORD Access,
		/* [annotation][out] */
		_Out_  HANDLE *pNTHandle);

	HRESULT(STDMETHODCALLTYPE *MakeResident)(
		ID3D12Device * This,
		UINT NumObjects,
		_In_reads_(NumObjects)  ID3D12Pageable *const *ppObjects);

	HRESULT(STDMETHODCALLTYPE *Evict)(
		ID3D12Device * This,
		UINT NumObjects,
		_In_reads_(NumObjects)  ID3D12Pageable *const *ppObjects);

	HRESULT(STDMETHODCALLTYPE *CreateFence)(
		ID3D12Device * This,
		UINT64 InitialValue,
		D3D12_FENCE_FLAGS Flags,
		REFIID riid,
		_COM_Outptr_  void **ppFence);

	HRESULT(STDMETHODCALLTYPE *GetDeviceRemovedReason)(
		ID3D12Device * This);

	void (STDMETHODCALLTYPE *GetCopyableFootprints)(
		ID3D12Device * This,
		_In_  const D3D12_RESOURCE_DESC *pResourceDesc,
		_In_range_(0, D3D12_REQ_SUBRESOURCES)  UINT FirstSubresource,
		_In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource)  UINT NumSubresources,
		UINT64 BaseOffset,
		_Out_writes_opt_(NumSubresources)  D3D12_PLACED_SUBRESOURCE_FOOTPRINT *pLayouts,
		_Out_writes_opt_(NumSubresources)  UINT *pNumRows,
		_Out_writes_opt_(NumSubresources)  UINT64 *pRowSizeInBytes,
		_Out_opt_  UINT64 *pTotalBytes);

	HRESULT(STDMETHODCALLTYPE *CreateQueryHeap)(
		ID3D12Device * This,
		_In_  const D3D12_QUERY_HEAP_DESC *pDesc,
		REFIID riid,
		_COM_Outptr_opt_  void **ppvHeap);

	HRESULT(STDMETHODCALLTYPE *SetStablePowerState)(
		ID3D12Device * This,
		BOOL Enable);

	HRESULT(STDMETHODCALLTYPE *CreateCommandSignature)(
		ID3D12Device * This,
		_In_  const D3D12_COMMAND_SIGNATURE_DESC *pDesc,
		_In_opt_  ID3D12RootSignature *pRootSignature,
		REFIID riid,
		_COM_Outptr_opt_  void **ppvCommandSignature);

	void (STDMETHODCALLTYPE *GetResourceTiling)(
		ID3D12Device * This,
		_In_  ID3D12Resource *pTiledResource,
		_Out_opt_  UINT *pNumTilesForEntireResource,
		_Out_opt_  D3D12_PACKED_MIP_INFO *pPackedMipDesc,
		_Out_opt_  D3D12_TILE_SHAPE *pStandardTileShapeForNonPackedMips,
		_Inout_opt_  UINT *pNumSubresourceTilings,
		_In_  UINT FirstSubresourceTilingToGet,
		_Out_writes_(*pNumSubresourceTilings)  D3D12_SUBRESOURCE_TILING *pSubresourceTilingsForNonPackedMips);

	LUID(STDMETHODCALLTYPE *GetAdapterLuid)(
		ID3D12Device * This);

	END_INTERFACE
} ID3D12DeviceVtbl;


typedef struct ID3D12GraphicsCommandListVtbl
{
	BEGIN_INTERFACE

	HRESULT(STDMETHODCALLTYPE *QueryInterface)(
		ID3D12GraphicsCommandList * This,
		REFIID riid,
		_COM_Outptr_  void **ppvObject);

	ULONG(STDMETHODCALLTYPE *AddRef)(
		ID3D12GraphicsCommandList * This);

	ULONG(STDMETHODCALLTYPE *Release)(
		ID3D12GraphicsCommandList * This);

	HRESULT(STDMETHODCALLTYPE *GetPrivateData)(
		ID3D12GraphicsCommandList * This,
		_In_  REFGUID guid,
		_Inout_  UINT *pDataSize,
		_Out_writes_bytes_opt_(*pDataSize)  void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateData)(
		ID3D12GraphicsCommandList * This,
		_In_  REFGUID guid,
		_In_  UINT DataSize,
		_In_reads_bytes_opt_(DataSize)  const void *pData);

	HRESULT(STDMETHODCALLTYPE *SetPrivateDataInterface)(
		ID3D12GraphicsCommandList * This,
		_In_  REFGUID guid,
		_In_opt_  const IUnknown *pData);

	HRESULT(STDMETHODCALLTYPE *SetName)(
		ID3D12GraphicsCommandList * This,
		_In_z_  LPCWSTR Name);

	HRESULT(STDMETHODCALLTYPE *GetDevice)(
		ID3D12GraphicsCommandList * This,
		REFIID riid,
		_COM_Outptr_opt_  void **ppvDevice);

	D3D12_COMMAND_LIST_TYPE(STDMETHODCALLTYPE *GetType)(
		ID3D12GraphicsCommandList * This);

	HRESULT(STDMETHODCALLTYPE *Close)(
		ID3D12GraphicsCommandList * This);

	HRESULT(STDMETHODCALLTYPE *Reset)(
		ID3D12GraphicsCommandList * This,
		_In_  ID3D12CommandAllocator *pAllocator,
		_In_opt_  ID3D12PipelineState *pInitialState);

	void (STDMETHODCALLTYPE *ClearState)(
		ID3D12GraphicsCommandList * This,
		_In_opt_  ID3D12PipelineState *pPipelineState);

	void (STDMETHODCALLTYPE *DrawInstanced)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT VertexCountPerInstance,
		_In_  UINT InstanceCount,
		_In_  UINT StartVertexLocation,
		_In_  UINT StartInstanceLocation);

	void (STDMETHODCALLTYPE *DrawIndexedInstanced)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT IndexCountPerInstance,
		_In_  UINT InstanceCount,
		_In_  UINT StartIndexLocation,
		_In_  INT BaseVertexLocation,
		_In_  UINT StartInstanceLocation);

	void (STDMETHODCALLTYPE *Dispatch)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT ThreadGroupCountX,
		_In_  UINT ThreadGroupCountY,
		_In_  UINT ThreadGroupCountZ);

	void (STDMETHODCALLTYPE *CopyBufferRegion)(
		ID3D12GraphicsCommandList * This,
		_In_  ID3D12Resource *pDstBuffer,
		UINT64 DstOffset,
		_In_  ID3D12Resource *pSrcBuffer,
		UINT64 SrcOffset,
		UINT64 NumBytes);

	void (STDMETHODCALLTYPE *CopyTextureRegion)(
		ID3D12GraphicsCommandList * This,
		_In_  const D3D12_TEXTURE_COPY_LOCATION *pDst,
		UINT DstX,
		UINT DstY,
		UINT DstZ,
		_In_  const D3D12_TEXTURE_COPY_LOCATION *pSrc,
		_In_opt_  const D3D12_BOX *pSrcBox);

	void (STDMETHODCALLTYPE *CopyResource)(
		ID3D12GraphicsCommandList * This,
		_In_  ID3D12Resource *pDstResource,
		_In_  ID3D12Resource *pSrcResource);

	void (STDMETHODCALLTYPE *CopyTiles)(
		ID3D12GraphicsCommandList * This,
		_In_  ID3D12Resource *pTiledResource,
		_In_  const D3D12_TILED_RESOURCE_COORDINATE *pTileRegionStartCoordinate,
		_In_  const D3D12_TILE_REGION_SIZE *pTileRegionSize,
		_In_  ID3D12Resource *pBuffer,
		UINT64 BufferStartOffsetInBytes,
		D3D12_TILE_COPY_FLAGS Flags);

	void (STDMETHODCALLTYPE *ResolveSubresource)(
		ID3D12GraphicsCommandList * This,
		_In_  ID3D12Resource *pDstResource,
		_In_  UINT DstSubresource,
		_In_  ID3D12Resource *pSrcResource,
		_In_  UINT SrcSubresource,
		_In_  DXGI_FORMAT Format);

	void (STDMETHODCALLTYPE *IASetPrimitiveTopology)(
		ID3D12GraphicsCommandList * This,
		_In_  D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology);

	void (STDMETHODCALLTYPE *RSSetViewports)(
		ID3D12GraphicsCommandList * This,
		_In_range_(0, D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
		_In_reads_(NumViewports)  const D3D12_VIEWPORT *pViewports);

	void (STDMETHODCALLTYPE *RSSetScissorRects)(
		ID3D12GraphicsCommandList * This,
		_In_range_(0, D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects,
		_In_reads_(NumRects)  const D3D12_RECT *pRects);

	void (STDMETHODCALLTYPE *OMSetBlendFactor)(
		ID3D12GraphicsCommandList * This,
		_In_reads_opt_(4)  const FLOAT BlendFactor[4]);

	void (STDMETHODCALLTYPE *OMSetStencilRef)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT StencilRef);

	void (STDMETHODCALLTYPE *SetPipelineState)(
		ID3D12GraphicsCommandList * This,
		_In_  ID3D12PipelineState *pPipelineState);

	void (STDMETHODCALLTYPE *ResourceBarrier)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT NumBarriers,
		_In_reads_(NumBarriers)  const D3D12_RESOURCE_BARRIER *pBarriers);

	void (STDMETHODCALLTYPE *ExecuteBundle)(
		ID3D12GraphicsCommandList * This,
		_In_  ID3D12GraphicsCommandList *pCommandList);

	void (STDMETHODCALLTYPE *SetDescriptorHeaps)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT NumDescriptorHeaps,
		_In_reads_(NumDescriptorHeaps)  ID3D12DescriptorHeap *const *ppDescriptorHeaps);

	void (STDMETHODCALLTYPE *SetComputeRootSignature)(
		ID3D12GraphicsCommandList * This,
		_In_opt_  ID3D12RootSignature *pRootSignature);

	void (STDMETHODCALLTYPE *SetGraphicsRootSignature)(
		ID3D12GraphicsCommandList * This,
		_In_opt_  ID3D12RootSignature *pRootSignature);

	void (STDMETHODCALLTYPE *SetComputeRootDescriptorTable)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT RootParameterIndex,
		_In_  D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor);

	void (STDMETHODCALLTYPE *SetGraphicsRootDescriptorTable)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT RootParameterIndex,
		_In_  D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor);

	void (STDMETHODCALLTYPE *SetComputeRoot32BitConstant)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT RootParameterIndex,
		_In_  UINT SrcData,
		_In_  UINT DestOffsetIn32BitValues);

	void (STDMETHODCALLTYPE *SetGraphicsRoot32BitConstant)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT RootParameterIndex,
		_In_  UINT SrcData,
		_In_  UINT DestOffsetIn32BitValues);

	void (STDMETHODCALLTYPE *SetComputeRoot32BitConstants)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT RootParameterIndex,
		_In_  UINT Num32BitValuesToSet,
		_In_reads_(Num32BitValuesToSet * sizeof(UINT))  const void *pSrcData,
		_In_  UINT DestOffsetIn32BitValues);

	void (STDMETHODCALLTYPE *SetGraphicsRoot32BitConstants)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT RootParameterIndex,
		_In_  UINT Num32BitValuesToSet,
		_In_reads_(Num32BitValuesToSet * sizeof(UINT))  const void *pSrcData,
		_In_  UINT DestOffsetIn32BitValues);

	void (STDMETHODCALLTYPE *SetComputeRootConstantBufferView)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT RootParameterIndex,
		_In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);

	void (STDMETHODCALLTYPE *SetGraphicsRootConstantBufferView)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT RootParameterIndex,
		_In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);

	void (STDMETHODCALLTYPE *SetComputeRootShaderResourceView)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT RootParameterIndex,
		_In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);

	void (STDMETHODCALLTYPE *SetGraphicsRootShaderResourceView)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT RootParameterIndex,
		_In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);

	void (STDMETHODCALLTYPE *SetComputeRootUnorderedAccessView)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT RootParameterIndex,
		_In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);

	void (STDMETHODCALLTYPE *SetGraphicsRootUnorderedAccessView)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT RootParameterIndex,
		_In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation);

	void (STDMETHODCALLTYPE *IASetIndexBuffer)(
		ID3D12GraphicsCommandList * This,
		_In_opt_  const D3D12_INDEX_BUFFER_VIEW *pView);

	void (STDMETHODCALLTYPE *IASetVertexBuffers)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT StartSlot,
		_In_  UINT NumViews,
		_In_reads_opt_(NumViews)  const D3D12_VERTEX_BUFFER_VIEW *pViews);

	void (STDMETHODCALLTYPE *SOSetTargets)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT StartSlot,
		_In_  UINT NumViews,
		_In_reads_opt_(NumViews)  const D3D12_STREAM_OUTPUT_BUFFER_VIEW *pViews);

	void (STDMETHODCALLTYPE *OMSetRenderTargets)(
		ID3D12GraphicsCommandList * This,
		_In_  UINT NumRenderTargetDescriptors,
		_In_opt_  const D3D12_CPU_DESCRIPTOR_HANDLE *pRenderTargetDescriptors,
		_In_  BOOL RTsSingleHandleToDescriptorRange,
		_In_opt_  const D3D12_CPU_DESCRIPTOR_HANDLE *pDepthStencilDescriptor);

	void (STDMETHODCALLTYPE *ClearDepthStencilView)(
		ID3D12GraphicsCommandList * This,
		_In_  D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView,
		_In_  D3D12_CLEAR_FLAGS ClearFlags,
		_In_  FLOAT Depth,
		_In_  UINT8 Stencil,
		_In_  UINT NumRects,
		_In_reads_(NumRects)  const D3D12_RECT *pRects);

	void (STDMETHODCALLTYPE *ClearRenderTargetView)(
		ID3D12GraphicsCommandList * This,
		_In_  D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView,
		_In_  const FLOAT ColorRGBA[4],
		_In_  UINT NumRects,
		_In_reads_(NumRects)  const D3D12_RECT *pRects);

	void (STDMETHODCALLTYPE *ClearUnorderedAccessViewUint)(
		ID3D12GraphicsCommandList * This,
		_In_  D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
		_In_  D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
		_In_  ID3D12Resource *pResource,
		_In_  const UINT Values[4],
		_In_  UINT NumRects,
		_In_reads_(NumRects)  const D3D12_RECT *pRects);

	void (STDMETHODCALLTYPE *ClearUnorderedAccessViewFloat)(
		ID3D12GraphicsCommandList * This,
		_In_  D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
		_In_  D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
		_In_  ID3D12Resource *pResource,
		_In_  const FLOAT Values[4],
		_In_  UINT NumRects,
		_In_reads_(NumRects)  const D3D12_RECT *pRects);

	void (STDMETHODCALLTYPE *DiscardResource)(
		ID3D12GraphicsCommandList * This,
		_In_  ID3D12Resource *pResource,
		_In_opt_  const D3D12_DISCARD_REGION *pRegion);

	void (STDMETHODCALLTYPE *BeginQuery)(
		ID3D12GraphicsCommandList * This,
		_In_  ID3D12QueryHeap *pQueryHeap,
		_In_  D3D12_QUERY_TYPE Type,
		_In_  UINT Index);

	void (STDMETHODCALLTYPE *EndQuery)(
		ID3D12GraphicsCommandList * This,
		_In_  ID3D12QueryHeap *pQueryHeap,
		_In_  D3D12_QUERY_TYPE Type,
		_In_  UINT Index);

	void (STDMETHODCALLTYPE *ResolveQueryData)(
		ID3D12GraphicsCommandList * This,
		_In_  ID3D12QueryHeap *pQueryHeap,
		_In_  D3D12_QUERY_TYPE Type,
		_In_  UINT StartIndex,
		_In_  UINT NumQueries,
		_In_  ID3D12Resource *pDestinationBuffer,
		_In_  UINT64 AlignedDestinationBufferOffset);

	void (STDMETHODCALLTYPE *SetPredication)(
		ID3D12GraphicsCommandList * This,
		_In_opt_  ID3D12Resource *pBuffer,
		_In_  UINT64 AlignedBufferOffset,
		_In_  D3D12_PREDICATION_OP Operation);

	void (STDMETHODCALLTYPE *SetMarker)(
		ID3D12GraphicsCommandList * This,
		UINT Metadata,
		_In_reads_bytes_opt_(Size)  const void *pData,
		UINT Size);

	void (STDMETHODCALLTYPE *BeginEvent)(
		ID3D12GraphicsCommandList * This,
		UINT Metadata,
		_In_reads_bytes_opt_(Size)  const void *pData,
		UINT Size);

	void (STDMETHODCALLTYPE *EndEvent)(
		ID3D12GraphicsCommandList * This);

	void (STDMETHODCALLTYPE *ExecuteIndirect)(
		ID3D12GraphicsCommandList * This,
		_In_  ID3D12CommandSignature *pCommandSignature,
		_In_  UINT MaxCommandCount,
		_In_  ID3D12Resource *pArgumentBuffer,
		_In_  UINT64 ArgumentBufferOffset,
		_In_opt_  ID3D12Resource *pCountBuffer,
		_In_  UINT64 CountBufferOffset);

	END_INTERFACE
} ID3D12GraphicsCommandListVtbl;


#endif
