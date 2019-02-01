#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "comsuppw.lib")

#include "DX_12Renderer.h"
#include <assert.h>


DX_12Renderer::DX_12Renderer()
{
}

DX_12Renderer::~DX_12Renderer()
{
}

Material * DX_12Renderer::makeMaterial(const std::string & name)
{
	return nullptr;
}

Mesh * DX_12Renderer::makeMesh()
{
	return nullptr;
}

VertexBuffer * DX_12Renderer::makeVertexBuffer(size_t size, VertexBuffer::DATA_USAGE usage)
{
	return nullptr;
}

ConstantBuffer * DX_12Renderer::makeConstantBuffer(std::string NAME, unsigned int location)
{
	return nullptr;
}

RenderState * DX_12Renderer::makeRenderState()
{
	return nullptr;
}

Technique * DX_12Renderer::makeTechnique(Material * m, RenderState * r)
{
	return nullptr;
}

Texture2D * DX_12Renderer::makeTexture2D()
{
	return nullptr;
}

Sampler2D * DX_12Renderer::makeSampler2D()
{
	return nullptr;
}

std::string DX_12Renderer::getShaderPath()
{
	return std::string();
}

std::string DX_12Renderer::getShaderExtension()
{
	return std::string();
}

int DX_12Renderer::initialize(unsigned int width, unsigned int height)
{
	HRESULT hr = E_FAIL;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "%s", SDL_GetError());
		exit(-1);
	}

	Uint32 flags = SDL_WINDOW_SHOWN;
	window = SDL_CreateWindow("DirectX_12", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

	wHnd = GetActiveWindow();
	assert(wHnd);

	//Debug layer
#ifdef _DEBUG
	hr = D3D12GetDebugInterface(IID_PPV_ARGS(&this->m_debugController));
	if (SUCCEEDED(hr))
		m_debugController->EnableDebugLayer();
	else
		PostMessageBoxOnError(hr, L"Unable to enable DX12 Debug layer: ", L"Warning", MB_ICONWARNING, wHnd);
#endif // _DEBUG

	//Find a suitable DX12 adapter
	IDXGIFactory5 * factory = NULL;
	IDXGIAdapter1 * adapter = _findDX12Adapter(factory);

	//create our d3d device or a warp device if no suitable adapter was found
	hr = _createDevice(&factory, &adapter);
	//Release objects that are no longer needed
	SafeRelease(&factory);
	SafeRelease(&adapter);

	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Unable to create DX12 Device: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}



	return 0;
}

void DX_12Renderer::setWinTitle(const char * title)
{
	SDL_SetWindowTitle(this->window, title);
}

int DX_12Renderer::shutdown()
{
	return 0;
}

void DX_12Renderer::setClearColor(float, float, float, float)
{
}

void DX_12Renderer::clearBuffer(unsigned int)
{
}

void DX_12Renderer::setRenderState(RenderState * ps)
{
}

void DX_12Renderer::submit(Mesh * mesh)
{
}

void DX_12Renderer::frame()
{
}

void DX_12Renderer::present()
{
}

IDXGIAdapter1 * DX_12Renderer::_findDX12Adapter(IDXGIFactory5 * factory)
{
	IDXGIAdapter1 * adapter		= NULL;

	//Create the factory and iterate through adapters, find and return the first adapter that supports DX12
	if (!factory)
		CreateDXGIFactory(IID_PPV_ARGS(&factory));
	assert(factory);

	for (UINT index = 0;; ++index)
	{
		adapter = NULL;
		if (DXGI_ERROR_NOT_FOUND == factory->EnumAdapters1(index, &adapter))
			break;
		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device), nullptr)))
			break;
		
		SafeRelease(&adapter);
	}

	return adapter;
}

HRESULT DX_12Renderer::_createDevice(IDXGIFactory5 ** ppFactory, IDXGIAdapter1 ** ppAdapter)
{
	HRESULT hr = E_FAIL;
	if (*ppAdapter)
	{
		hr = D3D12CreateDevice(*ppAdapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&this->m_device));
	}
	else
	{
		(*ppFactory)->EnumWarpAdapter(IID_PPV_ARGS(ppAdapter));
		hr = D3D12CreateDevice(*ppAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&this->m_device));
	}
	return hr;
}
