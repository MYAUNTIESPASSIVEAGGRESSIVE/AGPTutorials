#pragma once
#include <string>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;

class Renderer;

class Mesh
{
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;
	ID3D11Buffer* vBuffer = NULL;
	ID3D11Buffer* iBuffer = NULL;

	unsigned int indexCount = 0;

public:

	Mesh(Renderer& renderer, std::string objPath);
	void Render();
};

