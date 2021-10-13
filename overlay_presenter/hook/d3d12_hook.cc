#include "d3d12_hook.h"
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <dxgi1_4.h>

using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")

struct CD3DX12_DEFAULT {};
extern const DECLSPEC_SELECTANY CD3DX12_DEFAULT D3D12_DEFAULT;

struct CD3DX12_SHADER_BYTECODE : public D3D12_SHADER_BYTECODE
{
	CD3DX12_SHADER_BYTECODE() = default;
	explicit CD3DX12_SHADER_BYTECODE(const D3D12_SHADER_BYTECODE &o) noexcept :
		D3D12_SHADER_BYTECODE(o)
	{}
	CD3DX12_SHADER_BYTECODE(
		_In_ ID3DBlob* pShaderBlob) noexcept
	{
		pShaderBytecode = pShaderBlob->GetBufferPointer();
		BytecodeLength = pShaderBlob->GetBufferSize();
	}
	CD3DX12_SHADER_BYTECODE(
		const void* _pShaderBytecode,
		SIZE_T bytecodeLength) noexcept
	{
		pShaderBytecode = _pShaderBytecode;
		BytecodeLength = bytecodeLength;
	}
};

struct CD3DX12_BLEND_DESC : public D3D12_BLEND_DESC
{
	CD3DX12_BLEND_DESC() = default;
	explicit CD3DX12_BLEND_DESC(const D3D12_BLEND_DESC& o) noexcept :
		D3D12_BLEND_DESC(o)
	{}
	explicit CD3DX12_BLEND_DESC(CD3DX12_DEFAULT) noexcept
	{
		AlphaToCoverageEnable = FALSE;
		IndependentBlendEnable = FALSE;
		const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			RenderTarget[i] = defaultRenderTargetBlendDesc;
	}
};

struct CD3DX12_RASTERIZER_DESC : public D3D12_RASTERIZER_DESC
{
	CD3DX12_RASTERIZER_DESC() = default;
	explicit CD3DX12_RASTERIZER_DESC(const D3D12_RASTERIZER_DESC& o) noexcept :
		D3D12_RASTERIZER_DESC(o)
	{}
	explicit CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT) noexcept
	{
		FillMode = D3D12_FILL_MODE_SOLID;
		CullMode = D3D12_CULL_MODE_BACK;
		FrontCounterClockwise = FALSE;
		DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		DepthClipEnable = TRUE;
		MultisampleEnable = FALSE;
		AntialiasedLineEnable = FALSE;
		ForcedSampleCount = 0;
		ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}
	explicit CD3DX12_RASTERIZER_DESC(
		D3D12_FILL_MODE fillMode,
		D3D12_CULL_MODE cullMode,
		BOOL frontCounterClockwise,
		INT depthBias,
		FLOAT depthBiasClamp,
		FLOAT slopeScaledDepthBias,
		BOOL depthClipEnable,
		BOOL multisampleEnable,
		BOOL antialiasedLineEnable,
		UINT forcedSampleCount,
		D3D12_CONSERVATIVE_RASTERIZATION_MODE conservativeRaster) noexcept
	{
		FillMode = fillMode;
		CullMode = cullMode;
		FrontCounterClockwise = frontCounterClockwise;
		DepthBias = depthBias;
		DepthBiasClamp = depthBiasClamp;
		SlopeScaledDepthBias = slopeScaledDepthBias;
		DepthClipEnable = depthClipEnable;
		MultisampleEnable = multisampleEnable;
		AntialiasedLineEnable = antialiasedLineEnable;
		ForcedSampleCount = forcedSampleCount;
		ConservativeRaster = conservativeRaster;
	}
};

namespace hook
{
	D3D12Hook::D3D12Hook(IDXGISwapChain *pSwapChain)
	{
		LOGFILE("%s: Constructor begin\n", __func__);
		if (pSwapChain == NULL)
		{
			LOGFILE("%s: swapchain=NULL\n", __func__);
			return;
		}
		// Assign detoured swap chain to local variable
		m_pSwapChain = pSwapChain;
	}

	D3D12Hook::~D3D12Hook()
	{

	}

	struct VERTEX_SHADER_INPUT_V4_0 {
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT4 Col;
	};

	HRESULT D3D12Hook::CompileShaderV4_0(const char *entry, const char *target, ID3DBlob **ppCode)
	{
		const char *hlslcode =
			"Texture2D ShaderTexture : register(t0);\n"
			"SamplerState Sampler : register(s0);\n"
			"struct VS_INPUT {\n"
			"    float3 Pos : POSITION;\n"
			"    float2 Tex : TEXCOORD;\n"
			"    float4 Col : COLOR;\n"
			"};\n"
			"struct PS_INPUT {\n"
			"    float4 Pos : SV_POSITION;\n"
			"    float2 Tex : TEXCOORD;\n"
			"    float4 Col : COLOR;\n"
			"};\n"
			"PS_INPUT vsmain( VS_INPUT input ) {\n"
			"    PS_INPUT output;\n"
			"    output.Pos = float4(input.Pos, 1.0f);\n"
			"    output.Tex = input.Tex;\n"
			"    output.Col = input.Col;\n"
			"    return output;\n"
			"};\n"
			"float4 psmain( PS_INPUT input ) : SV_Target{\n"
			"    return ShaderTexture.Sample(Sampler, input.Tex);\n"
			"};\n"
			"technique11 EffectTech{\n"
			"    pass P0{\n"
			"        SetVertexShader( CompileShader( vs_4_0, vsmain() ) );\n"
			"        SetPixelShader( CompileShader( ps_4_0, psmain() ) );\n"
			"    }\n"
			"};\n"
			;
		DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		shaderFlags |= D3DCOMPILE_DEBUG;
#endif
		ID3DBlob *pError = NULL;
		HRESULT result = D3DCompile(hlslcode, lstrlenA(hlslcode) + 1, "overlay", NULL, NULL, entry, target, shaderFlags, 0, ppCode, &pError);
		if (FAILED(result))
		{
			LOGFILE("%s: D3DCompile failed 0x%x\n", __func__, result);
		}
		if (pError) pError->Release();
		return result;
	}

	HRESULT D3D12Hook::D3D12CreateResource()
	{
		// Initialize pipeline from hooked swapchain

		HRESULT result = m_pSwapChain->GetDevice(__uuidof(ID3D12Device), reinterpret_cast<void **>(&m_pDevice));
		LOGFILE("%s: result=0x%x device=0x%p\n", __func__, result, m_pDevice);
		if (FAILED(result) || m_pDevice == NULL)
		{
			LOGFILE("%s: GetDevice failed\n", __func__);
			return result;
		}
		//// Describe and create command queue
		//D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		//queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		//queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		//m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue));

		IDXGISwapChain3 *pSwapChain3;
		m_pSwapChain->QueryInterface(__uuidof(IDXGISwapChain3), reinterpret_cast<void **>(&pSwapChain3));
		UINT frameIndex = pSwapChain3->GetCurrentBackBufferIndex();
		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = 2;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ID3D12DescriptorHeap *m_rtvHeap;
		result = m_pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
		LOGFILE("%s: CreateDescriptorHeap result=0x%x heap=0x%p\n", __func__, result, m_rtvHeap);
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
		
		result = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator));
		LOGFILE("%s, CreateCommandAllocator result=0x%x alloc:0x%p\n", __func__, result, m_pCommandAllocator);

		// Load assets

		// Create shaders code
		ID3DBlob *pVertexCode = NULL;
		ID3DBlob *pPixelCode = NULL;
		result = CompileShaderV4_0("vsmain", "vs_4_0", &pVertexCode);
		LOGFILE("%s: CompileShaderV4_0 result=0x%x vertex=0x%p\n", __func__, result, pVertexCode);
		result = CompileShaderV4_0("psmain", "ps_4_0", &pPixelCode);
		LOGFILE("%s: CompileShaderV4_0 result=0x%x pixel=0x%p\n", __func__, result, pPixelCode);
		// Create shaders
		D3D12_SHADER_BYTECODE m_pVertexShader = CD3DX12_SHADER_BYTECODE(pVertexCode);
		LOGFILE("%s: CreateVertexShader result=0x%x vertex=0x%p\n", __func__, result, m_pVertexShader);
		D3D12_SHADER_BYTECODE m_pPixelShader = CD3DX12_SHADER_BYTECODE(pPixelCode);
		LOGFILE("%s: CreatePixelShader result=0x%x pixel=0x%p\n", __func__, result, m_pPixelShader);
		//if (pVertexCode) pVertexCode->Release();
		//if (pPixelCode) pPixelCode->Release();
		// Create input layout
		D3D12_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		ID3DBlob *pSignature = NULL;
		ID3DBlob *pError = NULL;
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.NumParameters = 0;
		rootSignatureDesc.pParameters = nullptr;
		rootSignatureDesc.NumStaticSamplers = 0;
		rootSignatureDesc.pStaticSamplers = nullptr;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignature, &pError);
		// following not work
		//result = D3DGetInputAndOutputSignatureBlob(pVertexCode->GetBufferPointer(), pVertexCode->GetBufferSize(), &pSignature);
		//LOGFILE("%s: D3DGetInputAndOutputSignatureBlob result=0x%x signature=0x%p\n", __func__, result, pSignature);
		result = m_pDevice->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature));
		LOGFILE("%s: CreateRootSignature result=0x%x signature=0x%p\n", __func__, result, m_pRootSignature);
		//if (pSignature) pSignature->Release();
		// Create vertex buffer
		VERTEX_SHADER_INPUT_V4_0 vertices[]{
			{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) },
		};
		D3D12_RESOURCE_DESC vertexResourceDesc = {};
		vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vertexResourceDesc.Alignment = 0;
		vertexResourceDesc.Width = sizeof(VERTEX_SHADER_INPUT_V4_0) * 4;
		vertexResourceDesc.Height = 1;
		vertexResourceDesc.DepthOrArraySize = 1;
		vertexResourceDesc.MipLevels = 1;
		vertexResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		vertexResourceDesc.SampleDesc = { 1, 0 };
		vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		vertexResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		D3D12_HEAP_PROPERTIES heapProp = {};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProp.CreationNodeMask = 1;
		heapProp.VisibleNodeMask = 1;
		result = m_pDevice->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pVertexBuffer));
		LOGFILE("%s: CreateBuffer result=0x%x vertex=0x%p\n", __func__, result, m_pVertexBuffer);
		D3D12_RANGE range = { 0, 0 };
		UINT8 *pData;
		m_pVertexBuffer->Map(0, &range, reinterpret_cast<void**>(&pData));
		memcpy(pData, vertices, sizeof(VERTEX_SHADER_INPUT_V4_0) * 4);
		m_pVertexBuffer->Unmap(0, nullptr);
		m_vertexBufferView.BufferLocation = m_pVertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(VERTEX_SHADER_INPUT_V4_0) * 4;
		m_vertexBufferView.SizeInBytes = sizeof(VERTEX_SHADER_INPUT_V4_0) * 4;
		LOGFILE("%s: CreateBufferView result=0x%x loc=0x%p\n", __func__, result, m_vertexBufferView.BufferLocation);
		
		//TODO: attach to graphics command list associated with original pipeline instead of new creation

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { layout, _countof(layout) };
		psoDesc.pRootSignature = m_pRootSignature;
		psoDesc.VS = m_pVertexShader;
		psoDesc.PS = m_pPixelShader;
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		result = m_pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pPipelineState));
		LOGFILE("%s: CreateGraphicsPipelineState result=0x%x state=0x%p\n", __func__, result, m_pPipelineState);

		return ERROR_CALL_NOT_IMPLEMENTED;
	}

	void D3D12Hook::D3D12DrawOverlay(unsigned char * pbData, DXGI_HOOK_RECT rect)
	{
		//allocator reset
		//command list reset
		//TODO: do something to command list
		m_pCommandList->SetGraphicsRootSignature(m_pRootSignature);
	}
}