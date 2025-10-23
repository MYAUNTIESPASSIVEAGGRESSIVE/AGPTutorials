#pragma once
#include <DirectXMath.h>

class Transform
{
public:

	DirectX::XMVECTOR position{ 0, 0, 0 };
	DirectX::XMVECTOR rotation{ 0, 0, 0 };
	DirectX::XMVECTOR scale{ 1, 1, 1 };

	DirectX::XMMATRIX GetWorldMatrix();

	void Translate(DirectX::XMVECTOR translation);
	void Rotate(DirectX::XMVECTOR inRotation);
};

