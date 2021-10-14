#include "d3d11_hook.h"
#include "d3d_hook.h"
#include <d3d11_3.h>

namespace hook
{
	D3D11Hook::D3D11Hook(IDXGISwapChain *pSwapChain)
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

	D3D11Hook::~D3D11Hook()
	{

	}

	HRESULT D3D11Hook::D3D11CreateResource()
	{
		if (m_pSwapChain == NULL)
		{
			LOGFILE("%s: swapchain=NULL\n", __func__);
			return E_INVALIDARG;
		}
		HRESULT result = m_pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void **>(&m_pDevice));
		if (FAILED(result) || m_pDevice == NULL)
		{
			LOGFILE("%s: GetDevice ID3D11Device failed 0x%x\n", __func__, result);
			return result;
		}
		result = m_pDevice->CreateDeferredContext(0, &m_pDeferredContext);
		if (FAILED(result) || m_pDeferredContext == NULL)
		{
			LOGFILE("%s: CreateDeferredContext failed 0x%x\n", __func__, result);
			// If device does not allowed deferred context, means only can be use immediate context
			// it must be save original immediate context resource state before drawing,
			// use our created resource and draw indexed directly, then change back to original context state
			// TODO: use D3D11HookContextState or ID3DDeviceContextState
			m_pImmediateState = new D3D11HookContextState();
		}
		m_pDevice->GetImmediateContext(&m_pImmediateContext);
		// Set RTV to device context from back buffer
		ID3D11Resource *pBackBuffer = NULL;
		//m_pDevice->CreateBuffer (or create owned texture 2d buffer)
		result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void **>(&pBackBuffer));
		if (FAILED(result) || pBackBuffer == NULL)
			LOGFILE("%s: GetBuffer failed 0x%x\n", __func__, result);
		else
		{
			result = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
			LOGFILE("%s: CreateRenderTargetView result=0x%x rtv=0x%p\n", __func__, result, m_pRenderTargetView);
			//m_pDeviceContext->OMGetRenderTargetsAndUnorderedAccessViews (or get exists RTVs and find device matched)
			//m_pDeviceContext->OMSetRenderTargetsAndUnorderedAccessViews(1, &m_pRenderTargetView, NULL, 0, D3D11_KEEP_UNORDERED_ACCESS_VIEWS, NULL, NULL);
			pBackBuffer->Release();
		}
		// Compile shaders
		ID3DBlob *pVertexCode = NULL;
		ID3DBlob *pPixelCode = NULL;
		result = CompileShaderV4_0("vsmain", "vs_4_0", &pVertexCode);
		LOGFILE("%s: CompileShaderV4_0 result=0x%x vertex=0x%p\n", __func__, result, pVertexCode);
		result = CompileShaderV4_0("psmain", "ps_4_0", &pPixelCode);
		LOGFILE("%s: CompileShaderV4_0 result=0x%x pixel=0x%p\n", __func__, result, pPixelCode);
		// Create shaders
		result = m_pDevice->CreateVertexShader(pVertexCode->GetBufferPointer(), pVertexCode->GetBufferSize(), NULL, &m_pVertexShader);
		LOGFILE("%s: CreateVertexShader result=0x%x vertex=0x%p\n", __func__, result, m_pVertexShader);
		result = m_pDevice->CreatePixelShader(pPixelCode->GetBufferPointer(), pPixelCode->GetBufferSize(), NULL, &m_pPixelShader);
		LOGFILE("%s: CreatePixelShader result=0x%x pixel=0x%p\n", __func__, result, m_pPixelShader);
		if (pPixelCode) pPixelCode->Release();
		// Create input layout
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		ID3DBlob *pSignature = NULL;
		result = D3DGetInputAndOutputSignatureBlob(pVertexCode->GetBufferPointer(), pVertexCode->GetBufferSize(), &pSignature);
		LOGFILE("%s: D3DGetInputAndOutputSignatureBlob result=0x%x signature=0x%p\n", __func__, result, pSignature);
		result = m_pDevice->CreateInputLayout(layout, _ARRAYSIZE(layout), pSignature->GetBufferPointer(), pSignature->GetBufferSize(), &m_pInputLayout);
		LOGFILE("%s: CreateInputLayout result=0x%x layout=0x%p\n", __func__, result, m_pInputLayout);
		if (pVertexCode) pVertexCode->Release();
		if (pSignature) pSignature->Release();
		//// Set shaders to device context
		//m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
		//m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
		//m_pDeviceContext->IASetInputLayout(m_pInputLayout);
		// Create vertex buffer
		VERTEX_SHADER_INPUT_V4_0 vertices[]{
			{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) },
		};
		D3D11_SUBRESOURCE_DATA vertexInitData = {};
		vertexInitData.pSysMem = vertices;
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = sizeof(VERTEX_SHADER_INPUT_V4_0) * 4;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		result = m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexInitData, &m_pVertexBuffer);
		LOGFILE("%s: CreateBuffer result=0x%x vertex=0x%p\n", __func__, result, m_pVertexBuffer);
		// Create index buffer (triangle list)
		WORD indices[6]{ 0,1,2,0,2,3 };
		D3D11_SUBRESOURCE_DATA indexInitData = {};
		indexInitData.pSysMem = indices;
		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.ByteWidth = sizeof(WORD) * 6;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		result = m_pDevice->CreateBuffer(&indexBufferDesc, &indexInitData, &m_pIndexBuffer);
		LOGFILE("%s: CreateBuffer result=0x%x index=0x%p\n", __func__, result, m_pIndexBuffer);
		//// Set buffers to device context
		//UINT stride = sizeof(VERTEX_SHADER_INPUT_V4_0);
		//UINT offset = 0;
		//m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		//m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		//m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Create pixel shader sampler (0-255 color space to 0.0~1.0 float point)
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		result = m_pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
		LOGFILE("%s: CreateSamplerState result=0x%x state=0x%p\n", __func__, result, m_pSamplerState);
		//m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);
		// Create blend state
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		result = m_pDevice->CreateBlendState(&blendDesc, &m_pBlendState);
		LOGFILE("%s: CreateBlendState result=0x%x state=0x%p\n", __func__, result, m_pBlendState);
		//float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//m_pDeviceContext->OMSetBlendState(m_pBlendState, blendFactor, 0xFFFFFFFF);

		return S_OK;
	}

	HRESULT D3D11Hook::D3D11ReleaseResource()
	{
		D3D11ReleaseShaderResource();
		if (m_pBlendState) m_pBlendState->Release();
		if (m_pSamplerState) m_pSamplerState->Release();
		if (m_pIndexBuffer) m_pIndexBuffer->Release();
		if (m_pVertexBuffer) m_pVertexBuffer->Release();
		if (m_pInputLayout) m_pInputLayout->Release();
		if (m_pPixelShader) m_pPixelShader->Release();
		if (m_pVertexShader) m_pVertexShader->Release();
		if (m_pRenderTargetView) m_pRenderTargetView->Release();
		if (m_pImmediateContext) m_pImmediateContext->Release();
		if (m_pImmediateState) delete m_pImmediateState;
		if (m_pDeferredContext) m_pDeferredContext->Release();
		if (m_pDevice) m_pDevice->Release();
		return S_OK;
	}

	void D3D11Hook::D3D11CreateShaderResource(unsigned char *pbData, DXGI_HOOK_RECT rect)
	{
		if (m_pDevice == NULL)
		{
			LOGFILE("%s: device is NULL\n", __func__);
			return;
		}
		// If shader resource view exists, it will be released
		D3D11ReleaseShaderResource();
		// Create texture2d resource from buffer data
		D3D11_SUBRESOURCE_DATA textureInitData = {};
		textureInitData.pSysMem = pbData;
		textureInitData.SysMemPitch = rect.width * 4;
		textureInitData.SysMemSlicePitch = 0;
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = rect.width;
		textureDesc.Height = rect.height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		HRESULT result = m_pDevice->CreateTexture2D(&textureDesc, &textureInitData, &m_pTexture2D);
		if (FAILED(result))
			return;
		// Create shader resource
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		result = m_pDevice->CreateShaderResourceView(m_pTexture2D, &srvDesc, &m_pShaderResourceView);
		if (FAILED(result))
		{
			m_pTexture2D->Release();
			m_pTexture2D = NULL;
		}
	}

	//TODO: try to use d3d12 way update texture bytes data
	void D3D11Hook::D3D11UpdateShaderResource(unsigned char *pbData, DXGI_HOOK_RECT rect)
	{
		// Create shader resource view if not exists
		if (m_pShaderResourceView == NULL)
		{
			D3D11CreateShaderResource(pbData, rect);
			return;
		}
		// Check texture 2d resource size match with rect size
		D3D11_TEXTURE2D_DESC textureDesc = {};
		m_pTexture2D->GetDesc(&textureDesc);
		if (textureDesc.Width == rect.width ||
			textureDesc.Height == rect.height)
		{
			// D3D11_USAGE_DEFAULT can not use Map to copy data, use D3D11_USAGE_DYNAMIC instead
			//D3D11_MAPPED_SUBRESOURCE mappedResource = {};
			//HRESULT result = m_pDeferredContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			//LOGFILE("%s Map result=0x%x\n", __func__, result);
			//if (SUCCEEDED(result))
			//{
			//	memcpy(mappedResource.pData, pbData, rect.width * rect.height * 4);
			//	m_pDeferredContext->Unmap(pTexture2D, 0);
			//}

			// D3D11_USAGE_DEFAULT texture can be update by ID3D11Device3 but is in immediate context
			//ID3D11Device3 *pDevice3 = NULL;
			//HRESULT result = m_pDevice->QueryInterface(__uuidof(ID3D11Device3), reinterpret_cast<void **>(&pDevice3));
			//LOGFILE("%s QueryInterface result=0x%x device3=0x%p\n", __func__, result, pDevice3);
			//ID3D11DeviceContext3 *pContext3 = NULL;
			//pDevice3->GetImmediateContext3(&pContext3);
			//result = pContext3->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, NULL);
			//if (SUCCEEDED(result))
			//{
			//	pDevice3->WriteToSubresource(pTexture2D, 0, NULL, pbData, rect.width * 4, 0);
			//}
			//if (pContext3) pContext3->Release();
			//if (pDevice3) pDevice3->Release();
		}
		// Otherwise re-create texture 2d for shader resource view
		else
		{
			D3D11ReleaseShaderResource();
			D3D11CreateShaderResource(pbData, rect);
		}
	}

	void D3D11Hook::D3D11ReleaseShaderResource()
	{
		if (m_pShaderResourceView != NULL)
		{
			if (m_pTexture2D != NULL)
			{
				m_pTexture2D->Release();
				m_pTexture2D = NULL;
			}
			m_pShaderResourceView->Release();
			m_pShaderResourceView = NULL;
		}
	}

	void D3D11Hook::D3D11DrawOverlayState(unsigned char *pbData, DXGI_HOOK_RECT rect)
	{
		if (m_pDeferredContext == NULL && m_pImmediateContext == NULL)
		{
			LOGFILE("%s: devicecontext is NULL\n", __func__);
			return;
		}

		ID3D11DeviceContext *pContext = NULL;
		// If device does not allowed deferred context, means only can be use immediate context
		// it must be save original immediate context resource state before drawing,
		// use our created resource and draw indexed directly, then change back to original context state
		if (m_pDeferredContext == NULL)
		{
			m_pImmediateState->SetState(m_pImmediateContext);
			pContext = m_pImmediateContext;
		}
		else
		{
			pContext = m_pDeferredContext;
		}

		pContext->OMSetRenderTargetsAndUnorderedAccessViews(1, &m_pRenderTargetView, NULL, 0, D3D11_KEEP_UNORDERED_ACCESS_VIEWS, NULL, NULL);
		// Set shaders to device context
		pContext->VSSetShader(m_pVertexShader, NULL, 0);
		pContext->PSSetShader(m_pPixelShader, NULL, 0);
		pContext->IASetInputLayout(m_pInputLayout);
		// Set buffers to device context
		UINT stride = sizeof(VERTEX_SHADER_INPUT_V4_0);
		UINT offset = 0;
		pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pContext->PSSetSamplers(0, 1, &m_pSamplerState);
		float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		pContext->OMSetBlendState(m_pBlendState, blendFactor, 0xFFFFFFFF);
		// Create texture2d resource from buffer data
		D3D11CreateShaderResource(pbData, rect);
		pContext->PSSetShaderResources(0, 1, &m_pShaderResourceView);
		D3D11ReleaseShaderResource();
		// Set viewport
		D3D11_VIEWPORT viewport = {};
		viewport.Width = (float)rect.width;
		viewport.Height = (float)rect.height;
		viewport.TopLeftX = (float)rect.left;
		viewport.TopLeftY = (float)rect.top;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;
		pContext->RSSetViewports(1, &viewport);

		// Draw indexed
		pContext->DrawIndexed(6, 0, 0);

		if (m_pDeferredContext == NULL)
		{
			m_pImmediateState->GetState(m_pImmediateContext);
		}
		else
		{
			// Force to execute deferred command by immediate context(may cause performance issue but seems fine)
			ID3D11CommandList *pCommandList = NULL;
			HRESULT result = m_pDeferredContext->FinishCommandList(TRUE, &pCommandList);
			if (SUCCEEDED(result) && pCommandList != NULL)
			{
				m_pImmediateContext->ExecuteCommandList(pCommandList, TRUE);
				pCommandList->Release();
			}
		}
	}

	void D3D11Hook::D3D11DrawOverlay(unsigned char *pbData, DXGI_HOOK_RECT rect)
	{
		if (m_pDeferredContext == NULL)
		{
			LOGFILE("%s: devicecontext is NULL\n", __func__);
			return;
		}

		m_pDeferredContext->OMSetRenderTargetsAndUnorderedAccessViews(1, &m_pRenderTargetView, NULL, 0, D3D11_KEEP_UNORDERED_ACCESS_VIEWS, NULL, NULL);
		// Set shaders to device context
		m_pDeferredContext->VSSetShader(m_pVertexShader, NULL, 0);
		m_pDeferredContext->PSSetShader(m_pPixelShader, NULL, 0);
		m_pDeferredContext->IASetInputLayout(m_pInputLayout);
		// Set buffers to device context
		UINT stride = sizeof(VERTEX_SHADER_INPUT_V4_0);
		UINT offset = 0;
		m_pDeferredContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		m_pDeferredContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		m_pDeferredContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pDeferredContext->PSSetSamplers(0, 1, &m_pSamplerState);
		float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_pDeferredContext->OMSetBlendState(m_pBlendState, blendFactor, 0xFFFFFFFF);
		// Create texture2d resource from buffer data
		D3D11CreateShaderResource(pbData, rect);
		m_pDeferredContext->PSSetShaderResources(0, 1, &m_pShaderResourceView);
		D3D11ReleaseShaderResource();
		// Set viewport
		D3D11_VIEWPORT viewport = {};
		viewport.Width = (float)rect.width;
		viewport.Height = (float)rect.height;
		viewport.TopLeftX = (float)rect.left;
		viewport.TopLeftY = (float)rect.top;
		viewport.MaxDepth = 1.0f;
		viewport.MinDepth = 0.0f;
		m_pDeferredContext->RSSetViewports(1, &viewport);

		// Draw indexed
		m_pDeferredContext->DrawIndexed(6, 0, 0);

		// Force to execute deferred command by immediate context(may cause performance issue but seems fine)
		ID3D11CommandList *pCommandList = NULL;
		HRESULT result = m_pDeferredContext->FinishCommandList(TRUE, &pCommandList);
		if (SUCCEEDED(result) && pCommandList != NULL)
		{
			m_pImmediateContext->ExecuteCommandList(pCommandList, TRUE);
			pCommandList->Release();
		}
	}

#pragma region Immediate Context State Implementation

	void D3D11HookContextState::GetState(ID3D11DeviceContext *pContext)
	{
		pContext->VSGetShader(&pVertexShader, NULL, 0);
		pContext->PSGetShader(&pPixelShader, NULL, 0);
		pContext->PSGetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, ppSamplerStates);
		pContext->PSGetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, ppShaderResourceViews);
		pContext->IAGetInputLayout(&pInputLayout);
		pContext->IAGetIndexBuffer(&pIndexBuffer, &indexBufferFormat, &indexBufferOffset);
		pContext->IAGetPrimitiveTopology(&primitiveTopology);
		pContext->IAGetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, ppVertexBuffers, vertexStrides, vertexOffsets);
		pContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, ppRenderTargetViews, &pDepthStencilView);
		pContext->OMGetBlendState(&pBlendState, blendStateFactor, &blendMask);
		pContext->RSGetState(&pRasterizerState);
		pContext->RSGetViewports(&viewports, pViewports);
	}

	void D3D11HookContextState::SetState(ID3D11DeviceContext *pContext)
	{
		pContext->VSSetShader(pVertexShader, NULL, 0);
		pContext->PSSetShader(pPixelShader, NULL, 0);
		pContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, ppSamplerStates);
		pContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, ppShaderResourceViews);
		pContext->IASetInputLayout(pInputLayout);
		pContext->IASetIndexBuffer(pIndexBuffer, indexBufferFormat, indexBufferOffset);
		pContext->IASetPrimitiveTopology(primitiveTopology);
		pContext->IASetVertexBuffers(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, ppVertexBuffers, vertexStrides, vertexOffsets);
		pContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, ppRenderTargetViews, pDepthStencilView);
		pContext->OMSetBlendState(pBlendState, blendStateFactor, blendMask);
		pContext->RSSetState(pRasterizerState);
		pContext->RSSetViewports(viewports, pViewports);
	}

	D3D11HookContextState::D3D11HookContextState()
	{

	}

	D3D11HookContextState::~D3D11HookContextState()
	{
		if (pVertexShader) pVertexShader->Release();
		if (pPixelShader) pPixelShader->Release();
		for (unsigned int i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++)
			if (ppSamplerStates[i]) ppSamplerStates[i]->Release();
		for (unsigned int i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++)
			if (ppShaderResourceViews[i]) ppShaderResourceViews[i]->Release();
		if (pInputLayout) pInputLayout->Release();
		if (pIndexBuffer) pIndexBuffer->Release();
		for (unsigned int i = 0; i < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; i++)
			if (ppVertexBuffers[i]) ppVertexBuffers[i]->Release();
		for (unsigned int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			if (ppRenderTargetViews[i]) ppRenderTargetViews[i]->Release();
		if (pDepthStencilView) pDepthStencilView->Release();
		if (pBlendState) pBlendState->Release();
		if (pRasterizerState) pRasterizerState->Release();
	}

#pragma endregion

}