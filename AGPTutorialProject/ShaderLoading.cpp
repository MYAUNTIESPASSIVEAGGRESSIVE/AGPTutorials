#include "ShaderLoading.h"

#include <d3d11.h> // D3D functionaility
#include <d3dcompiler.h> // Needed for shader reflection

#include "ReadData.h" // reading shader bytecode
#include "Debug.h" // error logging

namespace ShaderLoading
{
	// load vertex shader function
	long LoadVertexShader(std::string filename, ID3D11Device* dev, ID3D11VertexShader** outVS, ID3D11InputLayout** outIL)
	{
		HRESULT hr;
		auto shaderBytecode = DX::ReadData(std::wstring(filename.begin(), filename.end()).c_str());
		hr = dev->CreateVertexShader(
			shaderBytecode.data(), shaderBytecode.size(), NULL, outVS);
		
		if (FAILED(hr))
		{
			LOG("failed to load vertex shader" + filename + ".");
		}

		hr = ReflectVShaderInputLayout(shaderBytecode, dev, outIL);

		if (FAILED(hr))
		{
			LOG("Feiled to reflect vertex shader" + filename + ".");
			return hr;
		}

		return S_OK;
	}

	// load pixel shader function
	long LoadPixelShader(std::string filename, ID3D11Device* dev, ID3D11PixelShader** outPS)
	{
		auto shaderBytecode = DX::ReadData(std::wstring(filename.begin(), filename.end()).c_str());
		HRESULT hr = dev->CreatePixelShader(
			shaderBytecode.data(), shaderBytecode.size(), NULL, outPS);

		if (FAILED(hr))
		{
			LOG("failed to load pixel shader" + filename + ".");
		}

		return S_OK;
	}

	// shader reflection allows for the app to shader signature and determine some inputs
	int ReflectVShaderInputLayout(std::vector<uint8_t>& vShaderBytecode,
		ID3D11Device* dev, ID3D11InputLayout** outIL)
	{
		// interface for the reflection object
		ID3D11ShaderReflection* vShaderReflection = NULL;
		D3DReflect(vShaderBytecode.data(), vShaderBytecode.size(), IID_ID3D11ShaderReflection, (void**)&vShaderReflection);

		D3D11_SHADER_DESC desc;
		vShaderReflection->GetDesc(&desc);

		// Signature Param = type that stores data about specfic paramers
		D3D11_SIGNATURE_PARAMETER_DESC* signatureParamDesc = new D3D11_SIGNATURE_PARAMETER_DESC[desc.InputParameters]{ 0 };
		for (UINT i = 0; i < desc.InputParameters; i++)
		{
			vShaderReflection->GetInputParameterDesc(i, &signatureParamDesc[i]);
		}

		// signature param desc masks: float4 = 15, float3 = 7, float2 = 3, float[1] = 1. (bitmasks)
		// input element description - desctibes how the shader interperates vertex buffer data
		D3D11_INPUT_ELEMENT_DESC* ied = new D3D11_INPUT_ELEMENT_DESC[desc.InputParameters]{ 0 };
		for (size_t i = 0; i < desc.InputParameters; i++)
		{
			// this thing is disgusting.. too bad!!
			ied[i].SemanticName = signatureParamDesc[i].SemanticName;
			ied[i].SemanticIndex = signatureParamDesc[i].SemanticIndex;
			if (signatureParamDesc[i].ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
				switch (signatureParamDesc[i].Mask) {
				case 1: ied[i].Format = DXGI_FORMAT_R32_FLOAT;			 break; // float1
				case 3: ied[i].Format = DXGI_FORMAT_R32G32_FLOAT;		 break; // float2
				case 7: ied[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;	 break; // float3
				case 15: ied[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break; // float4
				default: break; // "oh no" - Sonic
				}
			} // ^^^ only covers ---x, --yx, -zyx, wzyx. may be possible for masks to be -yx- (6), etc.
			ied[i].InputSlot = 0;
			ied[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			ied[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			ied[i].InstanceDataStepRate = 0;
		}

		// creates input element COM object
		HRESULT result = dev->CreateInputLayout(ied, desc.InputParameters, 
			vShaderBytecode.data(), vShaderBytecode.size(), outIL);
		if (FAILED(result))
		{
			LOG("failed to create input layout");
			return result;
		}

		delete[] signatureParamDesc;
		delete[] ied;
		
		return S_OK;
	}
}
