#include "Camera.h"

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	DirectX::XMVECTOR eyePos = transform.position;
	DirectX::XMVECTOR lookAt = { 0, 0, 1 };
	DirectX::XMVECTOR camUp = { 0, 1, 0 };

	return DirectX::XMMatrixLookAtLH(eyePos, lookAt, camUp);
}

/*
	DirectX::XMMATRIX Camera::GetProjectionMatrix(int screenWidth, int screenHeight)
	{
		//return DirectX::XMMatrixPerspectiveFovLH(
		return NULL;
		//);
	}

*/
