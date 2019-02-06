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
	return std::string("..\\assets\\DX12\\");
}

std::string DX_12Renderer::getShaderExtension()
{
	return std::string(".hlsl");
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
	//create command queue
	hr = _createCommandQueue();
	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Unable to create DX12 Command Queue: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}
	//create command allocator and command list
	hr = _createCmdAllocatorAndList();
	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Error in _createCmdAllocatorAndList function: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}
	//create the swapchain
	hr = _createSwapChain(width, height, &factory);
	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Failed to create Swapchain: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}
	//create the render targets
	hr = _createRenderTargets();
	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Failed to create Render Targets: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}
	//create the fence
	hr = _createFence();
	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Failed to create Fence: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}
	//create the root signature
	hr = _createRootSignature();
	if (FAILED(hr))
	{
		PostMessageBoxOnError(hr, L"Failed to create Root Signature: ", L"Fatal error", MB_ICONERROR, wHnd);
		exit(-1);
	}
	//Release objects that are no longer needed
	SafeRelease(&factory);
	SafeRelease(&adapter);

	//Define the viewport and scissor rect
	{
		this->m_viewPort.TopLeftX		= 0.f;
		this->m_viewPort.TopLeftY		= 0.f;
		this->m_viewPort.MinDepth	= 0.f;
		this->m_viewPort.MaxDepth	= 1.f;
		this->m_viewPort.Width			= (float)width;
		this->m_viewPort.Height			= (float)height;

		this->m_scissorRect.left			= (long)this->m_viewPort.TopLeftX;
		this->m_scissorRect.top			= (long)this->m_viewPort.TopLeftY;
		this->m_scissorRect.right		= (long)this->m_viewPort.Width;
		this->m_scissorRect.bottom	= (long)this->m_viewPort.Height;
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

void DX_12Renderer::setClearColor(float r, float g, float b, float a)
{
	clearColor[0] = r; clearColor[1] = g; clearColor[2] = b; clearColor[3] = a;
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
	//clearColor[0] = float(rand() % 256) / 255.f; //just for basic testing
	this->PopulateCommandList();
	ID3D12CommandList* listsToExecute[] = { m_commandList };
	m_commandQueue->ExecuteCommandLists(_ARRAYSIZE(listsToExecute), listsToExecute);
}

void DX_12Renderer::present()
{
	DXGI_PRESENT_PARAMETERS pp = {};
	m_swapChain->Present1(0, 0, &pp);

	this->WaitForGPU();

	m_frameIndex = (m_frameIndex + 1) % FRAME_COUNT;
}

inline void DX_12Renderer::WaitForGPU()
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

inline void DX_12Renderer::PopulateCommandList()
{
	//get the handle for the current render target as backbuffer
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = m_rtvDescHeap->GetCPUDescriptorHandleForHeapStart();
	cdh.ptr += m_rtvDescSize * m_frameIndex;

	//Taken mostly from MSDN guide for basic dx12 component

	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
	ThrowIfFailed(this->m_commandAllocator->Reset());

	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before 
	// re-recording.
	ThrowIfFailed(m_commandList->Reset(m_commandAllocator, nullptr)); //we have no pipeline state object yet

	 // Set necessary state.
	m_commandList->SetGraphicsRootSignature(m_rootSignature);
	m_commandList->RSSetViewports(1, &m_viewPort);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);

	// Indicate that the back buffer will be used as a render target.
	SetResourceTransitionBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// Record commands.
	m_commandList->OMSetRenderTargets(1, &cdh, TRUE, nullptr);
	m_commandList->ClearRenderTargetView(cdh, clearColor, 0, nullptr);

	//Iterate through Mesh lists now?

	// Indicate that the back buffer will now be used to present.
	SetResourceTransitionBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	//close the list to prepare it for execution
	ThrowIfFailed(m_commandList->Close());
}

inline void DX_12Renderer::SetResourceTransitionBarrier(D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	D3D12_RESOURCE_BARRIER barrierDesc = {};

	barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrierDesc.Transition.pResource = m_renderTargets[m_frameIndex];
	barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrierDesc.Transition.StateBefore = beforeState;
	barrierDesc.Transition.StateAfter = afterState;

	m_commandList->ResourceBarrier(1, &barrierDesc);
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

HRESULT DX_12Renderer::_createRootSignature()
{
	HRESULT hr = E_FAIL;

	//Important things to identify: 
	// 1. How many root constants does this program need? (1 DWORD, 0 indirections)
	// 2. How many Root Descriptors does this program need/have (CBV, SRV, UAV)? (2 DWORDs, 1 indirection)
	// 3. How many Descriptor Tables do we need/want? (1 DWORD, 2 indirections)
	// 4. How many DWORDs did we use (Max size of 64, or 63 with enabled IA)

	unsigned int DWORD_COUNT = 0;
	//Start with defining descriptor ranges
	D3D12_DESCRIPTOR_RANGE descRanges[1]; //Lets start with one
	{
		//1 CBV descriptor = 2 DWORDs
		{	
			descRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
			descRanges[0].NumDescriptors = 1; //Only assmuing one ConstantBuffer so far
			descRanges[0].BaseShaderRegister = 0; // register b0
			descRanges[0].RegisterSpace = 0; //register(b0, space0);
			descRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			DWORD_COUNT += 2;
		}
	}
	//Create necessary Descriptor Tables
	D3D12_ROOT_DESCRIPTOR_TABLE descTables[1]; //Only need one so far
	{
		//1 Descriptor Table for the CBV descriptor = 1 DWORD
		{
			descTables[0].NumDescriptorRanges = _ARRAYSIZE(descRanges); //how many descriptors for this table
			descTables[0].pDescriptorRanges = &descRanges[0]; //pointer to descriptor array

			DWORD_COUNT += 1;
		}
	}

	//Create the root parameters (basically define the root table structure)
	D3D12_ROOT_PARAMETER rootParams[1]; //We only have defined one element to insert (Descriptor table)
	{
		// [0] - Descriptor table for CBV descriptor
		{
			rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //What type is the entry?
			rootParams[0].DescriptorTable = descTables[0]; //Which desc table?
			rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //Which shader stages can access this entry? 
		}
	}

	//Create the descriptions of the root signature
	D3D12_ROOT_SIGNATURE_DESC rsDesc;
	{
		rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; //IA enabled = max 63 DWORDs
		rsDesc.NumParameters = _ARRAYSIZE(rootParams); //How many entries?
		rsDesc.pParameters = rootParams; //Pointer to array of table entries
		rsDesc.NumStaticSamplers = 0;  //No static samplers were defined
		rsDesc.pStaticSamplers = NULL; //No static samplers were defined
	}

	//Serialize the root signature (no error blob)
	ID3DBlob * pSerBlob = NULL;
	D3D12SerializeRootSignature(
		&rsDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&pSerBlob,
		nullptr
	);

	//Use d3d12 device to create the root signature
	UINT nodeMask = 0;
	return this->m_device->CreateRootSignature(nodeMask, pSerBlob->GetBufferPointer(), pSerBlob->GetBufferSize(), IID_PPV_ARGS(&this->m_rootSignature));
}
