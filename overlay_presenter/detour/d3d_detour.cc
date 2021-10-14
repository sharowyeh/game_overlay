#include "d3d_detour.h"

#ifdef LOGFILE
#undef LOGFILE
#define LOGFILE(fmt, ...) WriteLog("detour.txt", fmt, ##__VA_ARGS__)
#endif

#include "../hook/d3d_hook.h"

namespace detour
{
	BOOL AttachD3D12(IDXGIAdapter* pAdapter)
	{
		BOOL bResult = FALSE;
		// Create D3D12Device from adapter and attach d3d12 interfaces
		ID3D12Device *m_pDevice = NULL;
		ID3D12CommandAllocator *pCommandAllocator = NULL;
		HRESULT result = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));
		LOGFILE("%s: D3D12CreateDevice result=0x%x device=0x%p\n", __func__, result, m_pDevice);
		if (m_pDevice == NULL)
			return FALSE;

		result = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCommandAllocator));
		LOGFILE("%s: CreateCommandAllocator result=0x%x allocator=0x%p\n", __func__, result, pCommandAllocator);

		//// Create shaders code
		//ID3DBlob *pVertexCode = NULL;
		//ID3DBlob *pPixelCode = NULL;
		//result = hook::CompileShaderV4_0("vsmain", "vs_4_0", &pVertexCode);
		//LOGFILE("%s: CompileShaderV4_0 result=0x%x vertex=0x%p\n", __func__, result, pVertexCode);
		//result = hook::CompileShaderV4_0("psmain", "ps_4_0", &pPixelCode);
		//LOGFILE("%s: CompileShaderV4_0 result=0x%x pixel=0x%p\n", __func__, result, pPixelCode);
		//// Create shaders
		//D3D12_SHADER_BYTECODE vertexShader = {};
		//vertexShader.pShaderBytecode = pVertexCode->GetBufferPointer();
		//vertexShader.BytecodeLength = pVertexCode->GetBufferSize();
		//LOGFILE("%s: CreateVertexShader result=0x%x vertex=0x%p\n", __func__, result, vertexShader.pShaderBytecode);
		//D3D12_SHADER_BYTECODE pixelShader = {};
		//pixelShader.pShaderBytecode = pPixelCode->GetBufferPointer();
		//pixelShader.BytecodeLength = pPixelCode->GetBufferSize();
		//LOGFILE("%s: CreatePixelShader result=0x%x pixel=0x%p\n", __func__, result, pixelShader.pShaderBytecode);
		////if (pVertexCode) pVertexCode->Release();
		////if (pPixelCode) pPixelCode->Release();
		//// Create input layout
		//D3D12_INPUT_ELEMENT_DESC layout[] =
		//{
		//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		//};
		//ID3DBlob *pSignature = NULL;
		//ID3DBlob *pError = NULL;
		//D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		//ID3D12RootSignature *pRootSignature = NULL;
		//rootSignatureDesc.NumParameters = 0;
		//rootSignatureDesc.pParameters = nullptr;
		//rootSignatureDesc.NumStaticSamplers = 0;
		//rootSignatureDesc.pStaticSamplers = nullptr;
		//rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		//D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignature, &pError);
		//result = m_pDevice->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&pRootSignature));
		//LOGFILE("%s: CreateRootSignature result=0x%x signature=0x%p\n", __func__, result, pRootSignature);
		////if (pSignature) pSignature->Release();
		//// Create vertex buffer
		//hook::VERTEX_SHADER_INPUT_V4_0 vertices[]{
		//	{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) },
		//	{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) },
		//	{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) },
		//	{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) },
		//};
		//D3D12_RESOURCE_DESC vertexResourceDesc = {};
		//vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		//vertexResourceDesc.Alignment = 0;
		//vertexResourceDesc.Width = sizeof(hook::VERTEX_SHADER_INPUT_V4_0) * 4;
		//vertexResourceDesc.Height = 1;
		//vertexResourceDesc.DepthOrArraySize = 1;
		//vertexResourceDesc.MipLevels = 1;
		//vertexResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		//vertexResourceDesc.SampleDesc = { 1, 0 };
		//vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//vertexResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		//D3D12_HEAP_PROPERTIES heapProp = {};
		//heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		//heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		//heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		//heapProp.CreationNodeMask = 1;
		//heapProp.VisibleNodeMask = 1;
		//ID3D12Resource *pVertexBuffer = NULL;
		//result = m_pDevice->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pVertexBuffer));
		//LOGFILE("%s: CreateBuffer result=0x%x vertex=0x%p\n", __func__, result, pVertexBuffer);
		//D3D12_RANGE range = { 0, 0 };
		//UINT8 *pData;
		//pVertexBuffer->Map(0, &range, reinterpret_cast<void**>(&pData));
		//memcpy(pData, vertices, sizeof(hook::VERTEX_SHADER_INPUT_V4_0) * 4);
		//pVertexBuffer->Unmap(0, nullptr);
		//D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
		//vertexBufferView.BufferLocation = pVertexBuffer->GetGPUVirtualAddress();
		//vertexBufferView.StrideInBytes = sizeof(hook::VERTEX_SHADER_INPUT_V4_0) * 4;
		//vertexBufferView.SizeInBytes = sizeof(hook::VERTEX_SHADER_INPUT_V4_0) * 4;
		//LOGFILE("%s: CreateBufferView result=0x%x loc=0x%p\n", __func__, result, vertexBufferView.BufferLocation);

		//// Describe and create the graphics pipeline state object (PSO).
		//ID3D12PipelineState *pPipelineState = NULL;
		//D3D12_RASTERIZER_DESC rsDesc = {};
		//rsDesc.FillMode = D3D12_FILL_MODE_SOLID;
		//rsDesc.CullMode = D3D12_CULL_MODE_BACK;
		//rsDesc.FrontCounterClockwise = FALSE;
		//rsDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		//rsDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		//rsDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		//rsDesc.DepthClipEnable = TRUE;
		//rsDesc.MultisampleEnable = FALSE;
		//rsDesc.AntialiasedLineEnable = FALSE;
		//rsDesc.ForcedSampleCount = 0;
		//rsDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		//D3D12_BLEND_DESC blendDesc = {};
		//blendDesc.AlphaToCoverageEnable = FALSE;
		//blendDesc.IndependentBlendEnable = FALSE;
		//const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
		//{
		//	FALSE,FALSE,
		//	D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		//	D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		//	D3D12_LOGIC_OP_NOOP,
		//	D3D12_COLOR_WRITE_ENABLE_ALL,
		//};
		//for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		//	blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;
		//D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		//psoDesc.InputLayout = { layout, _countof(layout) };
		//psoDesc.pRootSignature = pRootSignature;
		//psoDesc.VS = vertexShader;
		//psoDesc.PS = pixelShader;
		//psoDesc.RasterizerState = rsDesc;
		//psoDesc.BlendState = blendDesc;
		//psoDesc.DepthStencilState.DepthEnable = FALSE;
		//psoDesc.DepthStencilState.StencilEnable = FALSE;
		//psoDesc.SampleMask = UINT_MAX;
		//psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		//psoDesc.NumRenderTargets = 1;
		//psoDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
		//psoDesc.SampleDesc.Count = 1;
		//result = m_pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pPipelineState));
		//LOGFILE("%s: CreateGraphicsPipelineState result=0x%x state=0x%p\n", __func__, result, pPipelineState);
		//TODO: get failed result 0x80070057 here but commandlist can be attached
		ID3D12PipelineState *pPipelineState = NULL;
		ID3D12CommandList *pCommandList = NULL;
		m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCommandAllocator, pPipelineState, IID_PPV_ARGS(&pCommandList));
		LOGFILE("%s: CreateCommandList result=0x%x cmdlist=0x%p\n", __func__, result, pCommandList);
		// Command lists are created in the recording state, but there is nothing
		// to record yet. The main loop expects it to be closed, so close it now.
		auto pGraphCommand = reinterpret_cast<ID3D12GraphicsCommandList*>(pCommandList);
		pGraphCommand->Close();
		if (pCommandList != NULL)
			bResult = AttachD3D12CommandList(pGraphCommand);
		// attach major interface after method execution for dummy output instance 
		bResult = AttachD3D12Device(m_pDevice);
		return bResult;
	}

	BOOL AttachD3D11()
	{
		// Create a dummy window then assign HWND to override method
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_CLASSDC;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = L"dummy_class";
		wc.lpfnWndProc = DefWindowProc;
		RegisterClassEx(&wc);
		HWND hwnd = CreateWindow(L"dummy_class", L"dummy_window", NULL, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
		if (hwnd == NULL)
			return FALSE;
		// Notice: almost forgot D3D must needs HWND assigned to swap chain's output window,
		// and can not called from dllmain because related dll may not loaded
		ID3D11Device *pDevice = NULL;
		ID3D11DeviceContext *pContext = NULL;
		IDXGISwapChain *pSwapChain = NULL;
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		DXGI_MODE_DESC modeDesc = {};
		modeDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		modeDesc.Width = 1;
		modeDesc.Height = 1;
		modeDesc.RefreshRate.Numerator = 60;
		modeDesc.RefreshRate.Denominator = 1;
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferDesc = modeDesc;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		//HRESULT result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
		//	&featureLevel, 1, D3D11_SDK_VERSION, &pDevice, NULL, &pContext);
		//LOGFILE("%s: CreateDevice result=0x%x device=0x%p context=0x%p\n", __func__, result, pDevice, pContext);
		HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
			&featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, NULL, &pContext);
		LOGFILE("%s: CreateDeviceAndSwapChain result=0x%x device=0x%p swapchain=0x%p context=0x%p\n", __func__, result, pDevice, pSwapChain, pContext);
		BOOL bResult = FALSE;
		// Create DXGI objects and attach to d3d interfaces
		IDXGIDevice *pDXGIDevice = NULL;
		IDXGIAdapter *pDXGIAdapter = NULL;
		IDXGIFactory *pDXGIFactory = NULL;
		if (pDevice != NULL)
			result = pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void **>(&pDXGIDevice));
		if (pDXGIDevice != NULL)
		{
			result = pDXGIDevice->GetAdapter(&pDXGIAdapter);
			// interact d3d12 with dummy window before attachment
			if (result == S_OK && pDXGIAdapter != NULL) {
				// attach d3d12 via adapter
				bResult = AttachD3D12(pDXGIAdapter);
			}
			bResult = AttachDXGIDevice(pDXGIDevice);
			pDXGIDevice->Release();
		}
		if (pDXGIAdapter != NULL)
		{
			result = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&pDXGIFactory));
			bResult = AttachDXGIAdapter(pDXGIAdapter);
			pDXGIAdapter->Release();
		}
		if (pDXGIFactory != NULL)
		{
			bResult = AttachDXGIFactory(pDXGIFactory);
			pDXGIFactory->Release();
		}
		if (pSwapChain != NULL)
		{
			bResult = AttachDXGISwapChain(pSwapChain);
			pSwapChain->Release();
		}
		if (pContext != NULL)
		{
			bResult = AttachD3D11DeviceContext(pContext);
			pContext->Release();
		}
		if (pDevice != NULL)
		{
			bResult = AttachD3D11Device(pDevice);
			pDevice->Release();
		}
		// Close window or destory window
		DestroyWindow(hwnd);
		return bResult;
	}

	BOOL DetachD3D11()
	{
		DetachD3D12CommandList();
		DetachD3D12Device();
		DetachDXGIDevice();
		DetachDXGIAdapter();
		DetachDXGIFactory();
		DetachDXGISwapChain();
		DetachD3D11DeviceContext();
		DetachD3D11Device();
		return TRUE;
	}
}