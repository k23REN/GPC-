#include "TileMapComponent.h"
#include "Actor.h"
#include "Math.h"
#include<fstream>
#include<sstream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
{
	
}

void TileMapComponent::Load(const std::string aFilePath)
{
	//変数宣言時に引数ありのコンストラクタ呼び出しをすることで、
	//宣言時にファイルを呼び出す
	std::ifstream ifs(aFilePath);
	
	std::string rowStr;
	int row = 0;
	std::array<std::array<int, m_kMapTileWidth>, m_kMapTileHeight> layer;

	while (std::getline(ifs, rowStr)) {
		//std::getline関数で区切り文字を指定するときに必要な型
		std::istringstream stream(rowStr);//getlineで読み込んだ1行をコピーしてるのみ
		std::string columnStr;
		int column = 0;

		//第3引数の','を区切り文字とし、区切り文字までを抜き出してくれる
		while (std::getline(stream,columnStr,',')){
			std::istringstream ss(columnStr);
			ss >> layer[row][column];

			column++;
		}

		row++;
	}

	ifs.close();

	m_Layers.emplace_back(layer);
}

std::string TileMapComponent::GetData(const int aRow, const int aColumn)
{
	return std::string();
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	if (m_pTexture) {
		SDL_Rect r;

		r.w = static_cast<int>(m_kMapTileSize);
		r.h = static_cast<int>(m_kMapTileSize);

		r.x = 0;
		r.y = 0;

		SDL_Rect tile;

		tile.w = tile.h = m_kMapTileSize;

		for (auto&& layer : m_Layers) {
			for (int h = 0; h < m_kMapTileHeight; h++) {
				for (int w = 0; w < m_kMapTileWidth; w++) {
					tile.x = (layer[h][w] % (m_texWidth / m_kMapTileSize)) * m_kMapTileSize;
					tile.y = (layer[h][w] / (m_texWidth / m_kMapTileSize)) * m_kMapTileSize;

					r.x = w * m_kMapTileSize;
					r.y = h * m_kMapTileSize;

					SDL_RenderCopyEx(renderer,
									 m_pTexture,
									 &tile,
									 &r,
									 -Math::ToDegrees(m_pOwner->GetRotation()),
									 nullptr,
									 SDL_FLIP_NONE);
				}
			}
		}

	}
}
