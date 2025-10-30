#include "Transform.h"

DirectX::XMMATRIX Transform::GetWorldMatrix()
{
	DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScalingFromVector(scale);
	DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation);
	DirectX::XMMATRIX translationMat = DirectX::XMMatrixTranslationFromVector(position);
	return scaleMat * rotationMat * translationMat;
}

// translate function
void Transform::Translate(DirectX::XMVECTOR translation)
{
	position = DirectX::XMVectorAdd(position, translation);
}

// Rotate Function
void Transform::Rotate(DirectX::XMVECTOR inRotation)
{
	rotation = DirectX::XMVectorAddAngles(rotation, inRotation);
}

// get the formward vector
DirectX::XMVECTOR Transform::GetForward()
{
	float pitch = DirectX::XMVectorGetX(rotation);
	float yaw = DirectX::XMVectorGetY(rotation);

	DirectX::XMVECTOR direction
	{
		cosf(pitch) * sinf(yaw), // X
		sinf(pitch),			 // Y
		cosf(pitch) * cosf(yaw), // Z
		0.0f
	};

	return DirectX::XMVector3Normalize(direction);
}

// get the right vector
DirectX::XMVECTOR Transform::GetRight()
{
	float pitch = DirectX::XMVectorGetX(rotation);
	float yaw = DirectX::XMVectorGetY(rotation);
	float roll = DirectX::XMVectorGetZ(rotation);

	DirectX::XMVECTOR direction
	{
		cosf(roll) * cosf(yaw) + sinf(roll) * sinf(pitch) * sinf(yaw),  // X
		sinf(roll) * cosf(pitch),									    // Y
		cosf(roll) * -sinf(yaw) + sinf(roll) * sinf(pitch) * cosf(yaw), // Z
		0.0f
	};

	return DirectX::XMVector3Normalize(direction);
}

// get the up vector
DirectX::XMVECTOR Transform::GetUp()
{
	DirectX::XMVECTOR cross = DirectX::XMVector3Cross(GetForward(), GetRight());

	return DirectX::XMVector3Normalize(cross);
}
