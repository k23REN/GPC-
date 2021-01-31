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

	//csv�f�[�^�ǂݍ��݊֐�
	void Load(const std::string aFilePath);
	//�ǂݍ��񂾃f�[�^���擾����֐�
	std::string GetData(const int aRow, const int aColumn);

	void Draw(SDL_Renderer* renderer) override final;

private:
	static inline constexpr int m_kMapTileSize = 32;
	static inline constexpr int m_kMapTileWidth = 32;
	static inline constexpr int m_kMapTileHeight = 24;

	std::vector<std::array<std::array<int, m_kMapTileWidth>, m_kMapTileHeight>> m_Layers;
};

