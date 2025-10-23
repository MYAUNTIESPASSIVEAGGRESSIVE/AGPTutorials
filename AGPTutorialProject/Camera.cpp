#include "Camera.h"

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	DirectX::XMVECTOR eyePos = transform.position;
	DirectX::XMVECTOR lookAt = transform.GetForward();
	DirectX::XMVECTOR camUp = transform.GetUp();

	//return DirectX::XMMatrixLookAtLH(eyePos, lookAt, camUp); // looks in coord of (0,0,1)
	return DirectX::XMMatrixLookToLH(eyePos, lookAt, camUp); // looks foward (at the object)
}

// gets the projection matrix of the camera, set to perspective
DirectX::XMMATRIX Camera::GetProjectionMatrix(int screenWidth, int screenHeight)
{
	return DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(fov), // angle of the cams fov in radians
		screenWidth / (float)screenHeight, // aspect ratio
		nearClippingPlane, // close wont be rendered
		farClippingPlane); // far wont be rendered

	// look into XMMatrixOrthographicLH for ortho view
}
