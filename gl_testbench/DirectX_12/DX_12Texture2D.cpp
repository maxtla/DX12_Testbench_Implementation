#include "DX_12Texture2D.h"
#include "stb_image.h"
#include "d3dx12.h"

DX_12Texture2D::DX_12Texture2D(DX_12Renderer* renderer) : _renderer(renderer)
{
	// Describe and create a shader resource view (srv) heap for the texture
	// do not know where to put this
	/*D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	renderer->m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap));*/
}

DX_12Texture2D::~DX_12Texture2D()
{
	SafeRelease(&_texture);/*
	SafeRelease(&_srvHeap);*/
}

int DX_12Texture2D::loadFromFile(std::string filename)
{
	int w, h, bpp;
	unsigned char* rgb = stbi_load(filename.c_str(), &w, &h, &bpp, STBI_rgb_alpha);
	if (rgb == nullptr)
	{
		fprintf(stderr, "Error loading texture file: %s\n", filename.c_str());
		return -1;
	}

	SafeRelease(&_texture);

	// Release the pointer (_textureUploadHeap) only when the upload of the texture is done
	ID3D12Resource* _textureUploadHeap = nullptr;

	// Create the texture
	{
		// Describe and create a Texture2D
		D3D12_RESOURCE_DESC rd = {};
		rd.MipLevels = 1;
		rd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rd.Width = w;
		rd.Height = h;
		rd.Flags = D3D12_RESOURCE_FLAG_NONE;
		rd.DepthOrArraySize = 1;
		rd.SampleDesc.Count = 1;
		rd.SampleDesc.Quality = 0;
		rd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

		_renderer->m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&rd,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&_texture)
		);

		// Create the GPU upload buffer
		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(_texture, 0, 1);

		_renderer->m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&_textureUploadHeap)
		);

		// Copy data to the intermediate upload heap and then schedule a copy
		// from the upload heap to the Texture2D
		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = &rgb;
		textureData.RowPitch = w * 4; // 4 = size of pixel: rgba.
		textureData.SlicePitch = textureData.RowPitch * h;

		UpdateSubresources(_renderer->m_commandList, _texture, _textureUploadHeap, 0, 0, 1, &textureData);
		_renderer->m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(_texture, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

		// Describe and create a SRV for the texture
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = rd.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		_renderer->m_device->CreateShaderResourceView(_texture, &srvDesc, _srvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	//// Close the command list and execute it to begin the initial GPU setup.
	//_renderer->m_commandList->Close();
	//ID3D12CommandList* ppCommandLists[] = { _renderer->m_commandList };
	//_renderer->m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	//// Create synchronization objects and wait until assets have been uploaded to the GPU.
	//{
	//	_renderer->m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_renderer->m_fence));
	//	_renderer->m_fenceValue = 1;

	//	// Create an event handle to use for frame synchronization.
	//	_renderer->m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	//	if (_renderer->m_fenceEvent == nullptr)
	//	{
	//		// Error
	//	}

	//	// Wait for the command list to execute; we are reusing the same command 
	//	// list in our main loop but for now, we just want to wait for setup to 
	//	// complete before continuing.
	//	_renderer->WaitForGPU();
	//}

	SafeRelease(&_textureUploadHeap);

	stbi_image_free(rgb);
	return 0;
}

void DX_12Texture2D::bind(unsigned int slot)
{
}
