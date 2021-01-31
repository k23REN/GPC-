#pragma once
#include"SpriteComponent.h"
#include<string>
#include<vector>
#include<array>
class TileMapComponent : public SpriteComponent
{
public:
	TileMapComponent(class Actor* owner, int drawOrder = 100);
	~TileMapComponent(){};

	//csvデータ読み込み関数
	void Load(const std::string aFilePath);
	//読み込んだデータを取得する関数
	std::string GetData(const int aRow, const int aColumn);

	void Draw(SDL_Renderer* renderer) override final;

private:
	static inline constexpr int m_kMapTileSize = 32;
	static inline constexpr int m_kMapTileWidth = 32;
	static inline constexpr int m_kMapTileHeight = 24;

	std::vector<std::array<std::array<int, m_kMapTileWidth>, m_kMapTileHeight>> m_Layers;
};

