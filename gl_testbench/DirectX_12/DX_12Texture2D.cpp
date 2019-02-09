#include "DX_12Texture2D.h"
#include "stb_image.h"

std::map<std::string, ID3D12Resource*> DX_12Texture2D::s_textureMap; 
int DX_12Texture2D::RefID = 0;

DX_12Texture2D::DX_12Texture2D(DX_12Renderer * pRenderer)
{
	this->pDxRenderer = pRenderer;
}

DX_12Texture2D::~DX_12Texture2D()
{
	auto it = s_textureMap.begin();
	for (it; it != s_textureMap.end(); it++)
	{
		if (it->second == _texture) //name exists already
		{
			if (SafeRelease(&it->second) == 0); //Dereference -> Counter decreases by 1, if 0 it is to erased from map
			s_textureMap.erase(it);
			break;
		}
	}
	_texture = NULL;
}

int DX_12Texture2D::loadFromFile(std::string filename)
{
	//check the texture map if we already have loaded this texture from file
	if (_checkTextureMap(filename))
		return 0;

	int w, h, bpp;
	unsigned char* rgba = stbi_load(filename.c_str(), &w, &h, &bpp, STBI_rgb_alpha);
	if (rgba == nullptr)
	{
		fprintf(stderr, "Error loading texture file: %s\n", filename.c_str());
		return -1;
	}

	if (_texture)
		SafeRelease(&_texture);
	// Release the pointer (_textureUploadHeap) only when the upload of the texture is done
	ID3D12Resource* _textureUploadHeap = nullptr;

	// Create the texture
	{
		// Describe and create a Texture2D
		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.Width = w;
		textureDesc.Height = h;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Alignment = 0;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

		D3D12_HEAP_PROPERTIES heapProperties = {};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 1;
		heapProperties.VisibleNodeMask = 1;

		ThrowIfFailed( pDxRenderer->m_device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,// We will copy the texture from the upload heap to here, so we start it out in a copy dest state
			nullptr,
			IID_PPV_ARGS(&_texture)
		) );

		_texture->SetName(std::wstring(L"Texture2D_Resource_" + std::to_wstring(RefID)).c_str());
		RefID++;
		// Create the GPU upload buffer
		UINT64 uploadBufferSize = 0;
		{
			pDxRenderer->m_device->GetCopyableFootprints(
				&textureDesc,
				0,
				1,
				0,
				nullptr,
				nullptr,
				nullptr,
				&uploadBufferSize);
		}

		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

		D3D12_RESOURCE_DESC rdBuffer = {};
		rdBuffer.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		rdBuffer.Alignment = 0;
		rdBuffer.Width = uploadBufferSize;
		rdBuffer.Height = 1;
		rdBuffer.DepthOrArraySize = 1;
		rdBuffer.MipLevels = 1;
		rdBuffer.Format = DXGI_FORMAT_UNKNOWN;
		rdBuffer.SampleDesc.Count = 1;
		rdBuffer.SampleDesc.Quality = 0;
		rdBuffer.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		rdBuffer.Flags = D3D12_RESOURCE_FLAG_NONE;

		ThrowIfFailed( pDxRenderer->m_device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&rdBuffer,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&_textureUploadHeap)
		) );

		// Copy data to the intermediate upload heap and then schedule a copy
		// from the upload heap to the Texture2D
		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = rgba;
		textureData.RowPitch = w * 4 * sizeof(unsigned char); // 4 = size of pixel: rgba.
		textureData.SlicePitch = textureData.RowPitch * h;

		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = _texture;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;


		UpdateSubresources(pDxRenderer->m_commandList, _texture, _textureUploadHeap, 0, 0, 1, &textureData);
		pDxRenderer->m_commandList->ResourceBarrier(1, &barrier);

		// Describe and create a SRV for the texture
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		pDxRenderer->m_device->CreateShaderResourceView(_texture, &srvDesc, pDxRenderer->m_resourceHeap->GetCPUDescriptorHandleForHeapStart());
	}

	// Close the command list and execute it to begin the initial GPU setup.
	pDxRenderer->m_commandList->Close();
	ID3D12CommandList* ppCommandLists[] = { pDxRenderer->m_commandList };
	pDxRenderer->m_commandQueue->ExecuteCommandLists(_ARRAYSIZE(ppCommandLists), ppCommandLists);

	pDxRenderer->WaitForGPU();

	SafeRelease(&_textureUploadHeap);

	stbi_image_free(rgba);
	return 1;
}

void DX_12Texture2D::bind(unsigned int slot)
{
}

bool DX_12Texture2D::_checkTextureMap(std::string & str)
{
	auto it = s_textureMap.begin();
	for (it; it != s_textureMap.end(); it++)
	{
		if (strcmp(it->first.c_str(), str.c_str()) == 0) //name exists already
		{
			this->_texture = it->second;
			it->second->AddRef();
			return true;
		}
	} //name does not exist yet
	return false;
}
