#include "ModelLoader.h"

#include <fstream>
#include <sstream>

#include <windows.h> // For OutputDebugStringA

DirectX::XMFLOAT3 ModelLoader::ParseFloat3(std::string verts)
{
	DirectX::XMFLOAT3 out;
	std::istringstream ss{ verts };
	ss >> out.x;
	ss >> out.y;
	ss >> out.z;
	return out;
}

DirectX::XMFLOAT2 ModelLoader::ParseFloat2(std::string verts)
{
	DirectX::XMFLOAT2 out;
	std::istringstream ss{ verts };
	ss >> out.x;
	ss >> out.y;
	return out;
}

ModelLoader::FaceFormat ModelLoader::DetectFaceFormat(std::string face)
{
	// f 1 2 3				-> 0 forward slashes = just position
	// f 1/1 2/2 3/3		-> 3 slashes = position and texcoords
	// f 1/1/1 2/2/2 3/3/3	-> 6 slashes = position, texcoords, normals
	// f 1//1 2//2 3//3		-> 6 slashes with adjacent = position, normals

	int slashCount = 0;
	bool doubleSlash = false;

	for (size_t i = 0; i < face.size(); i++)
	{
		if (face[i] == '/')
		{
			slashCount++;
			if (i + 1 < face.size() && face[i + 1] == '/')
			{
				doubleSlash = true;
			}
		}
	}

	if (slashCount == 6)
		return doubleSlash ? FaceFormat::V_VN : FaceFormat::V_VT_VN;
	else if (slashCount == 3)
		return FaceFormat::V_VT;
	else if (slashCount == 0)
		return FaceFormat::V;
	else
		return FaceFormat::FORMAT_ERROR;
}

void ModelLoader::ParseFace(std::string face)
{
	if (format == FaceFormat::UNKNOWN)
		format = DetectFaceFormat(face);

	FaceIndices p1, p2, p3;
	int replaced = 0;
	switch (format)
	{
	case ModelLoader::FaceFormat::UNKNOWN:
		break;
	case ModelLoader::FaceFormat::V:
		break;
	case ModelLoader::FaceFormat::V_VT:
		replaced = sscanf_s(face.c_str(), "%u/%u %u/%u %u/%u",
			&p1.v, &p1.vt, &p2.v, &p2.vt, &p3.v, &p3.vt);
		if (replaced != 6)
			return;
		break;
	case ModelLoader::FaceFormat::V_VN:
		replaced = sscanf_s(face.c_str(), "%u//%u %u//%u %u//%u",
			&p1.v, &p1.vn, &p2.v, &p2.vn, &p3.v, &p3.vn);
		if (replaced != 6)
			return;
		break;
	case ModelLoader::FaceFormat::V_VT_VN:
		replaced = sscanf_s(face.c_str(), "%u/%u/%u %u/%u/%u %u/%u/%u",
				&p1.v, &p1.vt, &p1.vn, &p2.v, &p2.vt, &p2.vn, &p3.v, &p3.vt, &p3.vn);
		if (replaced != 9)
			return;
		break;

	case ModelLoader::FaceFormat::FORMAT_ERROR:
	default:
		return;
	}

	auto p1it = std::find(face_points.begin(), face_points.end(), p1);
	if (p1it == face_points.end())
	{
		face_points.push_back(p1);
		out_indices.push_back((unsigned int)face_points.size() - 1);
	}
	else
	{
		out_indices.push_back((unsigned int)std::distance(face_points.begin(), p1it));
	}

	auto p2it = std::find(face_points.begin(), face_points.end(), p2);
	if (p2it == face_points.end())
	{
		face_points.push_back(p2);
		out_indices.push_back((unsigned int)face_points.size() - 1);
	}
	else
	{
		out_indices.push_back((unsigned int)std::distance(face_points.begin(), p2it));
	}

	auto p3it = std::find(face_points.begin(), face_points.end(), p3);
	if (p3it == face_points.end())
	{
		face_points.push_back(p3);
		out_indices.push_back((unsigned int)face_points.size() - 1);
	}
	else
	{
		out_indices.push_back((unsigned int)std::distance(face_points.begin(), p3it));
	}
}

std::vector<std::string> ModelLoader::Split(std::string s, std::string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

ModelLoader::ModelLoader(std::string path)
{
	LoadModelData(path);
}

void ModelLoader::LoadModelData(std::string path)
{
	using namespace std;
	using namespace DirectX;

	ifstream file{ path, ifstream::in };

	string line; // Used to read file line by line
	while (getline(file, line))
	{
		istringstream ssLine{ line };
		string token; // Used to check first word of line for parsable data
		ssLine >> token;
		string lineData;
		if (token == "v")
		{
			getline(ssLine, lineData);
			XMFLOAT3 out = ParseFloat3(lineData);
			read_vertices.push_back(out);
		}
		else if (token == "vt")
		{
			getline(ssLine, lineData);
			XMFLOAT2 out = ParseFloat2(lineData);
			read_uv.push_back(out);
		}
		else if (token == "vn")
		{
			getline(ssLine, lineData);
			XMFLOAT3 out = ParseFloat3(lineData);
			read_normals.push_back(out);
		}
		else if (token == "f")
		{
			getline(ssLine, lineData);
			ParseFace(lineData);
		}
	}

	file.close();

	if (format == FaceFormat::FORMAT_ERROR)
	{
		std::string errorStr = "Unable to load " + path + ". Invalid face format";
		OutputDebugStringA((std::string(__FILE__) + "(" + std::to_string(__LINE__)
			+ "): " + std::string(errorStr) + "\n").c_str());
		return;
	}

	for (auto p : face_points)
	{
		VertexPosUVNorm v;
		v.pos = read_vertices[p.v - 1];
		if(format == FaceFormat::V_VT || format == FaceFormat::V_VT_VN)
			v.uv = read_uv[p.vt - 1];
		if (format == FaceFormat::V_VN || format == FaceFormat::V_VT_VN)
			v.norm = read_normals[p.vn - 1];
		out_verts.push_back(v);
	}

	// additional work!!!
	//read_vertices.clear();
	//read_uv.clear();
	//read_normals.clear();
	//face_points.clear();
}