#pragma once
#include <string>

class Renderer;

struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

class Texture
{
private:

	ID3D11ShaderResourceView* texture = nullptr; // texture
	ID3D11SamplerState* sampler = nullptr; // sampler


public:
	
	enum class TextureType
	{
		Texture2D,
		Cubemap
	};

	ID3D11ShaderResourceView* GetTexture() { return texture; }
	ID3D11SamplerState* GetSampler() { return sampler; }

	Texture(Renderer& renderer, std::string path, bool transparent = false, TextureType type = TextureType::Texture2D);
	~Texture();

	bool isTransparent = false;
};

