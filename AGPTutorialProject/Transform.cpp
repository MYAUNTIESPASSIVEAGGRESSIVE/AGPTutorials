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
