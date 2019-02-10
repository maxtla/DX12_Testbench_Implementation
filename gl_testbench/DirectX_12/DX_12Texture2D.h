#pragma once

#include "../Texture2D.h"
#include "DX_12Sampler2D.h"
#include "DX_12Renderer.h"
#include <map>

class DX_12Texture2D :
	public Texture2D
{
public:
	DX_12Texture2D(DX_12Renderer * pRenderer);
	~DX_12Texture2D();

	int loadFromFile(std::string filename);
	void bind(unsigned int slot);
	
	ID3D12Resource* _texture = nullptr;
private:
	//We do not need to dublicate the same textures, this acts as a Texture manager
	static std::map<std::string, ID3D12Resource*> s_textureMap;
	static int RefID;
	bool _checkTextureMap(std::string & str);


	DX_12Renderer * pDxRenderer = NULL;
};