#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL/SDL_log.h>
#include "Math.h"
#include "Renderer.h"

Mesh::Mesh()
	:m_pVertexArray(nullptr)
	, m_radius(0.0f)
	, m_specPower(100.0f)
{
}

Mesh::~Mesh()
{
}

bool Mesh::Load(const std::string& a_fileName, Renderer* a_renderer)
{
	std::ifstream file(a_fileName);
	if (!file.is_open()) {
		SDL_Log("File not found: Mesh %s", a_fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject()) {
		SDL_Log("Mesh %s is not valid json", a_fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	if (ver != 1) {
		SDL_Log("Mesh %s not version 1", a_fileName.c_str());
		return false;
	}

	m_shaderName = doc["shader"].GetString();

	size_t vertSize = 8;

	//! テクスチャ読み込み
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1) {
		SDL_Log("Mesh %s has no textures, there should be at least one", a_fileName.c_str());
		return false;
	}

	m_specPower = static_cast<float>(doc["specularPower"].GetDouble());

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++) {
		std::string texName = textures[i].GetString();
		Sptr<Texture> t = a_renderer->GetTexture(texName);
		if (t == nullptr) {
			t = a_renderer->GetTexture(texName);

			//! 読み込めなかった場合デフォルトテクスチャを読み込む
			if (t == nullptr) {
				t = a_renderer->GetTexture("../Assets/Default.png");
			}
		}
		m_pTextures.emplace_back(t);
	}

	// 頂点配列を読み込む
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1) {
		SDL_Log("Mesh %s has no vertices", a_fileName.c_str());
		return false;
	}

	std::vector<float> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	m_radius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++) {
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != 8) {
			SDL_Log("Unexpected vertex format for %s", a_fileName.c_str());
			return false;
		}

		Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		m_radius = Math::Max(m_radius, pos.LengthSq());

		for (rapidjson::SizeType i = 0; i < vert.Size(); i++) {
			vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
		}
	}

	m_radius = Math::Sqrt(m_radius);

	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1) {
		SDL_Log("Mesh %s has no indices", a_fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++) {
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3) {
			SDL_Log("Invalid indices for %s", a_fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	m_pVertexArray = std::make_shared<VertexArray>(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
								   indices.data(), static_cast<unsigned>(indices.size()));
	return true;
}

void Mesh::Unload()
{
	m_pVertexArray = nullptr;
}

Sptr<Texture> Mesh::GetTexture(size_t a_index)
{
	if (a_index < m_pTextures.size()) {
		return m_pTextures[a_index];
	}
	else {
		return nullptr;
	}
}
