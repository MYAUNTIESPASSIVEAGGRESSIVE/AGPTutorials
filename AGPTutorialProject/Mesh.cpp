#include "Mesh.h"
#include <d3d11.h>

#include "Renderer.h"
#include "ModelLoader.h"
#include "Debug.h"

Mesh::Mesh(Renderer& renderer, std::string objPath)
	: dev(renderer.GetDevice()), devcon(renderer.GetDeviceContext())
{
	ModelLoader ml{ objPath };

	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DYNAMIC; // allows for CPU-write and GPU-read
	//bdesc.ByteWidth = sizeof(Vertex) * 3; // size of buffer - sizeof vertex * num of vertices
	bd.ByteWidth = (unsigned int)ml.GetVertexBufferSize(); // can use this but only in local scope
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // use as vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // allow CPU to write in buffer
	if (FAILED(dev->CreateBuffer(&bd, NULL, &vBuffer)))
	{
		LOG("failed to create vertex buffer");
		return;
	}

	//copy the verticies into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // map the buffer
	memcpy(ms.pData, ml.GetVertexData(), ml.GetVertexBufferSize()); // copy the data into the buffer
	devcon->Unmap(vBuffer, NULL);

	// fill in a buffer description
	D3D11_BUFFER_DESC indexBD = { 0 };
	indexBD.Usage = D3D11_USAGE_DEFAULT;
	indexBD.ByteWidth = ml.GetIndexBufferSize();
	indexBD.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// define the rosource data.
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = ml.GetIndexData();

	indexCount = (int)ml.GetIndexCount();

	if (FAILED(dev->CreateBuffer(&indexBD, &initData, &iBuffer)))
	{
		LOG("failed to create index buffer");
		return;
	}
}

void Mesh::Render()
{
	// select which primitive we are using
	devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// select which vertex buffer to use
	UINT stride = sizeof(VertexPosUVNorm);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset); // This tells the rendering context which vertex buffers should be used.
	devcon->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0); // This tells the rendering context which vertex buffers should be used.

	devcon->DrawIndexed(indexCount, 0, 0);
}
