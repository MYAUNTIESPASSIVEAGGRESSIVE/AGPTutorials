#pragma once
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

struct ID3D11Buffer;

class Renderer;
class Texture;
class GameObject;

class Material
{
public:
	Material(std::string name, Renderer& renderer, 
		std::string vShaderFilename, std::string pShaderFilename,
		Texture* texture);

	Texture* GetTexture() { return texture; }

	virtual void Bind(); // set rendering state + bind then draw material

	virtual void UpdateMaterial(GameObject* entiry) {} // updating cbuffer

	~Material();

protected:

	struct CBufferBase // base mat type
	{

	};

	std::string name;

	Renderer& renderer;

	ID3D11Device* dev = nullptr;
	ID3D11DeviceContext* devcon = nullptr;
	ID3D11VertexShader* vShader = nullptr;
	ID3D11PixelShader* pShader = nullptr;
	ID3D11InputLayout* vLayout = nullptr;

	Texture* texture = nullptr;

	ID3D11Buffer* cbuffer = nullptr;

	void CreateCBuffer(unsigned int byteWidth, ID3D11Buffer*& targetBuffer);
	void UpdateCBuffer(CBufferBase& cbData, ID3D11Buffer*& targetBuffer);


};

