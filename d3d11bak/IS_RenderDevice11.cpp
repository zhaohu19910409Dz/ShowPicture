// IS_RenderDevice11.cpp

#include <d3dx11.h>
#include "IS_RenderDevice11.h"
#include "IS_Log.h"
#include "IS_VertexBuffer.h"
#include "IS_IndexBuffer.h"
#include "IS_ConstantBuffer.h"
#include "IS_Texture2D.h"
#include "IS_String.h"
#include "IS_VertexShader.h"
#include "IS_PixelShader.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dxgi.lib")

using namespace IS;

RenderDevice11::RenderDevice11()
	: IsInitialized(false)
{
}

RenderDevice11::~RenderDevice11()
{
	IsInitialized = false;
}

ErrorCode RenderDevice11::Initialize(void* hwnd, int w, int h)
{
	Window = (HWND)hwnd;
	ScreenWidth = w;
	ScreenHeight = h;

	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&DXGIFactory);
	if (FAILED(hr))
	{
		Log_Error(String(_T("CreateDXGIFactory failed.")));
		return EC_CreateDeviceFailed;
	}

	hr = DXGIFactory->EnumAdapters(0, &Adapter);
	if (FAILED(hr))
	{
		Log_Error(String(_T("EnumAdapters failed.")));
		return EC_CreateDeviceFailed;
	}

	hr = Adapter->EnumOutputs(0, &FullscreenOutput);
	if (FAILED(hr))
	{
		Log_Error(String(_T("EnumOutputs failed.")));
		return EC_CreateDeviceFailed;
	}

	unsigned int numModes = 0;
	hr = FullscreenOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(hr))
	{
		Log_Error(String(_T("GetDisplayModeList failed.")));
		return EC_CreateDeviceFailed;
	}

	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		Log_Error(String(_T("Malloc memory failed.")));
		return EC_MallocFailed;
	}

	hr = FullscreenOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(hr))
	{
		Log_Error(String(_T("GetDisplayModeList failed.")));
		return EC_CreateDeviceFailed;
	}

	unsigned int numerator = 0;
	unsigned int denominator = 1;
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)ScreenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)ScreenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	DXGI_ADAPTER_DESC adapterDesc = { 0 };
	hr = Adapter->GetDesc(&adapterDesc);
	if (FAILED(hr))
	{
		Log_Error(String(_T("Adapter getDesc failed.")));
		return EC_CreateDeviceFailed;
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	D3D_FEATURE_LEVEL featureLevel;

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = ScreenWidth;
	swapChainDesc.BufferDesc.Height = ScreenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (false)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = Window;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (false)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &Device, NULL, &Context);
	if (FAILED(hr))
	{
		Log_Error(String(_T("D3D11CreateDeviceAndSwapChain failed.")));
		return EC_CreateDeviceFailed;
	}

	ID3D11Texture2D* backBuffer = 0;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
	{
		Log_Error(String(_T("SwapChain getBuffer failed.")));
		return EC_CreateDeviceFailed;
	}

	hr = Device->CreateRenderTargetView(backBuffer, NULL, &RenderTargetView);
	if (FAILED(hr))
	{
		Log_Error(String(_T("CreateRenderTargetView failed.")));
		return EC_CreateDeviceFailed;
	}

	backBuffer->Release();
	backBuffer = 0;

	D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
	depthBufferDesc.Width = ScreenWidth;
	depthBufferDesc.Height = ScreenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = Device->CreateTexture2D(&depthBufferDesc, NULL, &DepthStencilBuffer);
	if (FAILED(hr))
	{
		Log_Error(String(_T("Create depthBuffer failed.")));
		return EC_CreateDeviceFailed;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { 0 };
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = Device->CreateDepthStencilState(&depthStencilDesc, &DepthStencilState);
	if (FAILED(hr))
	{
		Log_Error(String(_T("CreateDepthStencilState failed.")));
		return EC_CreateDeviceFailed;
	}

	Context->OMSetDepthStencilState(DepthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = Device->CreateDepthStencilView(DepthStencilBuffer, &depthStencilViewDesc, &DepthStencilView);
	if (FAILED(hr))
	{
		Log_Error(String(_T("CreateDepthStencilView failed.")));
		return EC_CreateDeviceFailed;
	}

	Context->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

	D3D11_RASTERIZER_DESC rasterDesc;
	memset(&rasterDesc, 0, sizeof(rasterDesc));

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	hr = Device->CreateRasterizerState(&rasterDesc, &RasterState);
	if (FAILED(hr))
	{
		Log_Error(String(_T("CreateRasterizerState failed.")));
		return EC_CreateDeviceFailed;
	}

	Context->RSSetState(RasterState);

	D3DViewPort.Width = (float)ScreenWidth;
	D3DViewPort.Height = (float)ScreenHeight;
	D3DViewPort.MinDepth = 0.0f;
	D3DViewPort.MaxDepth = 1.0f;
	D3DViewPort.TopLeftX = 0.0f;
	D3DViewPort.TopLeftY = 0.0f;

	Context->RSSetViewports(1, &D3DViewPort);

	IsInitialized = true;
	return EC_None;
}

Buffer* RenderDevice11::CreateBuffer(BufferType type, void* data, int perSize, int count)
{
	switch (type)
	{
	case BT_Vertex:
	{
		VertexBuffer* buffer = new VertexBuffer();
		buffer->BufferCount = count;
		buffer->perSize = perSize;

		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = perSize * count;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData;
		memset(&vertexData, 0, sizeof(vertexData));

		vertexData.pSysMem = data;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		HRESULT hr = Device->CreateBuffer(&vertexBufferDesc, &vertexData, &(buffer->BufferData));
		if (FAILED(hr))
		{
			Log_Error(String(_T("CreateBuffer failed.")));
			return 0;
		}

		return buffer;
	}
	break;
	case BT_Index:
	{
		IndexBuffer* buffer = new IndexBuffer();
		buffer->BufferCount = count;
		buffer->perSize = perSize;

		D3D11_BUFFER_DESC indexBufferDesc = {0};
		memset(&indexBufferDesc, 0, sizeof(indexBufferDesc));

		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = perSize * count;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData = {0};
		memset(&indexData, 0, sizeof(indexData));

		indexData.pSysMem = data;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		HRESULT hr = Device->CreateBuffer(&indexBufferDesc, &indexData, &(buffer->BufferData));
		if (FAILED(hr))
		{
			Log_Error(String(_T("CreateBuffer failed.")));
			return 0;
		}

		return buffer;
	}
	break;
	case BT_Constant:
	{
		ConstantBuffer* buffer = new ConstantBuffer();
		buffer->BufferCount = count;
		buffer->perSize = perSize;

		D3D11_BUFFER_DESC bufferDesc = { 0 };
		memset(&bufferDesc, 0, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = perSize * count;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA constantData = { 0 };
		memset(&constantData, 0, sizeof(constantData));

		constantData.pSysMem = data;
		constantData.SysMemPitch = 0;
		constantData.SysMemSlicePitch = 0;

		HRESULT hr = Device->CreateBuffer(&bufferDesc, &constantData, &(buffer->BufferData));
		if (FAILED(hr))
		{
			Log_Error(String(_T("CreateBuffer failed.")));
			return 0;
		}

		return buffer;
	}
	break;
	default:
		return 0;
	}

}

Texture2D* RenderDevice11::CreateTexture(TextureType type, String filename)
{
	switch (type)
	{
	case TT_Texture1D:
	{
		return 0;
	}
	break;
	case TT_Texture2D:
	{
		Texture2D* texture = new Texture2D();

		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(Device, filename.GetData(), NULL, NULL, &(texture->TexSv), NULL);
		if (FAILED(hr))
		{
			Log_Error(String(_T("D3DX11CreateShaderResourceViewFromFile failed.")));
			return 0;
		}

		texture->TexSv->GetResource((ID3D11Resource**)&(texture->Tex));

		D3D11_TEXTURE2D_DESC desc = { 0 };
		texture->Tex->GetDesc(&desc);

		texture->Width = desc.Width;
		texture->Height = desc.Height;
		texture->Samples = desc.SampleDesc.Count;

		switch (desc.Format)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			texture->Format = TF_RGBA32;
		default:
			texture->Format = TF_Unknow;
			break;
		}

		return texture;
	}
	default:
		return 0;
	}
}

Shader* RenderDevice11::CreateShader(ShaderStage stage, String filename)
{
	switch (stage)
	{
	case SS_Vertex:
	{
		VertexShader* shader = new VertexShader();
		
		ID3D10Blob* vertexShaderBuffer = 0;
		ID3D10Blob* errorMessage = 0;
		HRESULT hr = D3DX11CompileFromFile(filename.GetData(), NULL, NULL, \
			"main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, \
			&vertexShaderBuffer, &errorMessage, NULL);
		if (FAILED(hr))
		{
			if (errorMessage)
			{
				Log_Error(filename);
			}
			else
			{
				Log_Error(String(_T("Missing Shader File.")));
			}

			return 0;
		}

		hr = Device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), \
			NULL, &(shader->D3DShader));
		if (FAILED(hr))
		{
			Log_Error(String(_T("CreateVertexShader failed.")));
			return 0;
		}

		D3D11_INPUT_ELEMENT_DESC polygonLayout[2] = { {0} };

		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "TEXCOORD";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		hr = Device->CreateInputLayout(polygonLayout, numElements, \
			vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
			&(shader->VertexIL));
		if (FAILED(hr))
		{
			Log_Error(String(_T("CreateInputLayout failed.")));
			return false;
		}
		
		vertexShaderBuffer->Release();
		vertexShaderBuffer = 0;

		shader->D3DBuffer = CreateBuffer(BT_Constant, 0, sizeof(float) * 36, 1);

		return shader;
	}
	break;
	case SS_Pixel:
	{
		PixelShader* shader = new PixelShader();

		ID3D10Blob* errorMessage = 0;
		ID3D10Blob* pixelShaderBuffer = 0;

		HRESULT hr = D3DX11CompileFromFile(filename.GetData(), NULL, NULL, \
			"main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
			&pixelShaderBuffer, &errorMessage, NULL);
		if (FAILED(hr))
		{
			if (errorMessage)
			{
				Log_Error(filename);
			}
			else
			{
				Log_Error(String(_T("CreateVertexShader failed.")));
			}

			return 0;
		}

		hr = Device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), \
			pixelShaderBuffer->GetBufferSize(), NULL, &(shader->D3DShader));
		if (FAILED(hr))
		{
			Log_Error(String(_T("CreatePixelShader failed.")));
			return 0;
		}

		pixelShaderBuffer->Release();
		pixelShaderBuffer = 0;

		return shader;
	}
	break;
	default:
		return 0;
	}
}

void RenderDevice11::Render(Buffer* vertex, Buffer* index, Texture* tex, Shader* vs, Shader* ps)
{
	float color[4];
	color[0] = 0.16f;
	color[1] = 0.16f;
	color[2] = 0.16f;
	color[3] = 0.16f;

	Context->ClearRenderTargetView(RenderTargetView, color);
	Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	VertexBuffer* vb = (VertexBuffer*)vertex;
	stride = vb->perSize;
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	Context->IASetVertexBuffers(0, 1, &((VertexBuffer*)vertex)->BufferData, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	Context->IASetIndexBuffer(((IndexBuffer*)index)->BufferData, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Context->IASetInputLayout(((VertexShader*)vs)->VertexIL);

	// Set shader texture resource in the pixel shader.
	Context->PSSetShaderResources(0, 1, &(((Texture2D*)tex)->TexSv));

	// Set the vertex and pixel shaders that will be used to render this triangle.
	Context->VSSetShader(((VertexShader*)vs)->D3DShader, NULL, 0);
	Context->PSSetShader(((PixelShader*)ps)->D3DShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	//Context->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	Context->DrawIndexed(((IndexBuffer*)index)->BufferCount, 0, 0);

	SwapChain->Present(0, 0);
}