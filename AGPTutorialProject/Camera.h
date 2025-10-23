#pragma once
#include "Transform.h"
#include "DirectXMath.h"

class Camera
{

public:
	Transform transform;

	float fov = 65;
	float nearClippingPLane = 0.1f;
	float farClippingPLane = 100.0f;

	DirectX::XMMATRIX GetViewMatrix();

	DirectX::XMMATRIX GetProjectionMatrix(int screenWidth, int screenHeight);
};

