#pragma once
#include <vector>
#include <string>
#include "VertexArray.h"
#include "Texture.h"
#include "Utility.h"

class Renderer;

class Mesh
{
public:
	Mesh();
	~Mesh();

	/**
	* @brief メッシュのロード/アンロード
	*/
	bool Load(const std::string& a_fileName, Renderer* a_renderer);
	void Unload();

	///
	///取得
	///
	[[nodiscard]] Sptr<VertexArray> GetVertexArray() { return m_pVertexArray; }
	[[nodiscard]] Sptr<Texture> GetTexture(size_t a_index);
	[[nodiscard]] std::string_view GetShaderName() const { return m_shaderName; }
	[[nodiscard]] float GetRadius() const { return m_radius; }
	[[nodiscard]] float GetSpecPower() const { return m_specPower; }

private:
	std::vector<Sptr<Texture>> m_pTextures; //テクスチャ
	Sptr<VertexArray> m_pVertexArray;
	std::string m_shaderName;
	float m_radius;
	float m_specPower;
};