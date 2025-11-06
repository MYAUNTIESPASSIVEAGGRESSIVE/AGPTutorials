#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>

struct VertexPosUVNorm
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 norm;
};

class ModelLoader
{
public:
	enum class FaceFormat
	{
		UNKNOWN, V, V_VT, V_VN, V_VT_VN, FORMAT_ERROR
	};

	ModelLoader(std::string path);
	VertexPosUVNorm* GetVertexData() { return out_verts.data(); }
	size_t GetVertexCount() { return out_verts.size(); }
	size_t GetVertexBufferSize() { return out_verts.size() * sizeof(VertexPosUVNorm); }

	unsigned int* GetIndexData() { return out_indices.data(); }
	size_t GetIndexCount() { return out_indices.size(); }
	size_t GetIndexBufferSize() { return out_indices.size() * sizeof(unsigned int); }

private:
	struct FaceIndices
	{
		// 0 is erroneous here as OBJ indices start with 1
		unsigned int v = 0;
		unsigned int vt = 0;
		unsigned int vn = 0;

		FaceIndices() = default;
		FaceIndices(unsigned int v, unsigned int vt, unsigned int vn)
			: v(v), vt(vt), vn(vn) {	}

		bool operator==(const FaceIndices& other) const
		{
			return (v == other.v) && (vt == other.vt) && (vn == other.vn);
		}
	};

	FaceFormat format;

	// raw data
	std::vector<DirectX::XMFLOAT3> read_vertices;
	std::vector<DirectX::XMFLOAT2> read_uv;
	std::vector<DirectX::XMFLOAT3> read_normals;
	std::vector<FaceIndices> face_points;

	// for rendering
	std::vector<VertexPosUVNorm> out_verts;
	std::vector<unsigned int> out_indices;

	void LoadModelData(std::string path);

	FaceFormat DetectFaceFormat(std::string face);
	void ParseFace(std::string face);
	DirectX::XMFLOAT3 ParseFloat3(std::string verts);
	DirectX::XMFLOAT2 ParseFloat2(std::string verts);

	std::vector<std::string> Split(std::string s, std::string delimiter);
};