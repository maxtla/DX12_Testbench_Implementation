#pragma once

#include "../Renderer.h"

#include <SDL.h>
#include <d3d12.h>
#include <dxgi1_5.h>
#include <comdef.h>
#include <tchar.h>

class DX_12Renderer : public Renderer
{
public:
	DX_12Renderer();
	~DX_12Renderer();

	Material* makeMaterial(const std::string& name);
	Mesh* makeMesh();
	//VertexBuffer* makeVertexBuffer();
	VertexBuffer* makeVertexBuffer(size_t size, VertexBuffer::DATA_USAGE usage);
	ConstantBuffer* makeConstantBuffer(std::string NAME, unsigned int location);
	//	ResourceBinding* makeResourceBinding();
	RenderState* makeRenderState();
	Technique* makeTechnique(Material* m, RenderState* r);
	Texture2D* makeTexture2D();
	Sampler2D* makeSampler2D();
	std::string getShaderPath();
	std::string getShaderExtension();

	int initialize(unsigned int width = 640, unsigned int height = 480);
	void setWinTitle(const char* title);
	int shutdown();

	void setClearColor(float, float, float, float);
	void clearBuffer(unsigned int);
	//	void setRenderTarget(RenderTarget* rt); // complete parameters
	void setRenderState(RenderState* ps);
	void submit(Mesh* mesh);
	void frame();
	void present();

private:
	static const UINT FRAME_COUNT = 2;

	SDL_Window * window											    = NULL;
	HWND wHnd															    = NULL;
	//TODO: Add all relevant D3D components here	       
	//D3D12 Debug														    
	ID3D12Debug* m_debugController						    = NULL;
	//D3D12/Pipeline objects										    
	ID3D12Device* m_device										    = NULL;
	ID3D12CommandQueue* m_commandQueue		    = NULL;
	ID3D12CommandAllocator * m_commandAllocator	= NULL;
	IDXGISwapChain3* m_swapChain								= NULL;
	ID3D12DescriptorHeap* m_rtvDescHeap				    = NULL;
	ID3D12Resource* m_renderTargets[FRAME_COUNT]	= { NULL, NULL };
	//ID3D12RootSignature* m_rootSignature					= NULL; //I think this should be elsewhere
	//ID3D12PipelineState* m_pipelineState                       = NULL; //I think this should be elsewhere 
	ID3D12GraphicsCommandList* m_commandList       = NULL;
	UINT m_rtvDescSize													= 0;


	//Synchronization objects
	UINT m_frameIndex												= 0;
	HANDLE m_fenceEvent										= NULL;
	ID3D12Fence* m_fence										= NULL;
	UINT64 m_fenceValue											= 0;

	std::vector<Mesh*> drawList;
	std::unordered_map<Technique*, std::vector<Mesh*>> drawList2;

	bool globalWireframeMode = false;
	float clearColor[4] = { 0,0,0,0 };

private:
	//private functions for code seperation
	void WaitForGPU();
	//The factory is created in this function, if it is not already
	IDXGIAdapter1 * _findDX12Adapter(IDXGIFactory5 ** factory);
	HRESULT _createDevice(IDXGIFactory5 ** ppFactory, IDXGIAdapter1 ** ppAdapter);
	HRESULT _createCommandQueue();
	HRESULT _createCmdAllocatorAndList();
	HRESULT _createSwapChain(unsigned int width, unsigned int height, IDXGIFactory5 ** ppFactory);
	HRESULT _createRenderTargets();
	HRESULT _createFence();
};

//Since no default SafeRelease function is included in D3D12.h
//and I do not want to have include the ComPtr header to soley access theirs
//I decided to write their implementation directly in my header inlined
template <class T> inline void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

static void PostMessageBoxOnError(HRESULT hr, TCHAR* info, TCHAR* caption, UINT type, HWND hWnd)
{
	_com_error err(hr);
	TCHAR message[64]; //buffer to write to when concatenating the TCHAR strings
	_stprintf(message, _T("%s%s"), info, err.ErrorMessage());
	MessageBoxW(hWnd, message, caption, MB_OK | type);
}