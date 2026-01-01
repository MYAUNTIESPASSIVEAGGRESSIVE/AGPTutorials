#pragma once

#include <DirectXMath.h>

#define MAX_POINT_LIGHTS 8

struct PointLight
{
	DirectX::XMVECTOR position{ 0,0,0 };
	DirectX::XMVECTOR colour{ 1,1,1 };

	float strength = 10;
	bool enabled = false;
	float padding[2];
};

struct DirectionalLight
{
	DirectX::XMVECTOR directionFrom{ 0,0,0 };
	DirectX::XMVECTOR colour{ 1,1,1 };
};