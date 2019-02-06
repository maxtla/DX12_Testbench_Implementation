#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "comsuppw.lib")

#include "DX_12Renderer.h"
#include <assert.h>
#include "DX_12VertexBuffer.h"

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
	return new DX_12VertexBuffer(this, size, usage);
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
	IDXGIAdapter1 * adapter = _findDX12Adapter(&factory);

	//create our d3d device or a warp device if no suitable adapter was found
	hr = _createDevice(&factory, &adapter);
	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Unable to create DX12 Device: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}

	hr = _createCommandQueue();
	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Unable to create DX12 Command Queue: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}

	hr = _createCmdAllocatorAndList();
	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Error in _createCmdAllocatorAndList function: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}

	hr = _createSwapChain(width, height, &factory);
	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Failed to create Swapchain: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}

	hr = _createRenderTargets();
	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Failed to create Render Targets: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}

	hr = _createFence();
	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Failed to create Fence: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}
	//Release objects that are no longer needed
	SafeRelease(&factory);
	SafeRelease(&adapter);

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

void DX_12Renderer::WaitForGPU()
{
	const UINT64 fence = m_fenceValue;
	m_commandQueue->Signal(m_fence, fence);
	m_fenceValue++;

	if (m_fence->GetCompletedValue() < fence)
	{
		m_fence->SetEventOnCompletion(fence, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}

IDXGIAdapter1 * DX_12Renderer::_findDX12Adapter(IDXGIFactory5 ** ppFactory)
{
	IDXGIAdapter1 * adapter		= NULL;

	//Create the factory and iterate through adapters, find and return the first adapter that supports DX12
	if (!*ppFactory)
		CreateDXGIFactory(IID_PPV_ARGS(ppFactory));
	assert(*ppFactory);

	for (UINT index = 0;; ++index)
	{
		adapter = NULL;
		if (DXGI_ERROR_NOT_FOUND == (*ppFactory)->EnumAdapters1(index, &adapter))
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

HRESULT DX_12Renderer::_createCommandQueue()
{
	//Description
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	return  m_device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_commandQueue));
}

HRESULT DX_12Renderer::_createCmdAllocatorAndList()
{
	HRESULT hr = E_FAIL;

	hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));

	if (FAILED(hr))
		return hr;

	hr = m_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_commandAllocator,
		nullptr,
		IID_PPV_ARGS(&m_commandList)
	);

	if (SUCCEEDED(hr))
		m_commandList->Close();

	return hr;
}

HRESULT DX_12Renderer::_createSwapChain(unsigned int width, unsigned int height, IDXGIFactory5 ** ppFactory)
{
	HRESULT hr = E_FAIL;

	//Description
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = FRAME_COUNT;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = 0;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;


	hr = (*ppFactory)->CreateSwapChainForHwnd(
		m_commandQueue,
		wHnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		reinterpret_cast<IDXGISwapChain1**>(&m_swapChain)
	);

	return hr;
}

HRESULT DX_12Renderer::_createRenderTargets()
{
	HRESULT hr = E_FAIL;

	//Description for descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC dhd = {};
	dhd.NumDescriptors = FRAME_COUNT;
	dhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	hr = m_device->CreateDescriptorHeap(&dhd, IID_PPV_ARGS(&m_rtvDescHeap));
	if (FAILED(hr))
		return hr;

	//Create resources for render targets
	m_rtvDescSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = m_rtvDescHeap->GetCPUDescriptorHandleForHeapStart();

	//one RTV for each swapchain buffer
	for (UINT i = 0; i < FRAME_COUNT; i++)
	{
		hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
		if (SUCCEEDED(hr))
		{
			m_device->CreateRenderTargetView(m_renderTargets[i], nullptr, cdh);
			cdh.ptr += m_rtvDescSize;
		}
	}
	return hr;
}

HRESULT DX_12Renderer::_createFence()
{
	HRESULT hr = E_FAIL;
	hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	if (FAILED(hr))
		return hr;
	m_fenceValue = 1;

	m_fenceEvent = CreateEvent(0, false, false, 0);

	return hr;
}
