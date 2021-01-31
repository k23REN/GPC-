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
	//�ϐ��錾���Ɉ�������̃R���X�g���N�^�Ăяo�������邱�ƂŁA
	//�錾���Ƀt�@�C�����Ăяo��
	std::ifstream ifs(aFilePath);
	
	std::string rowStr;
	int row = 0;
	std::array<std::array<int, m_kMapTileWidth>, m_kMapTileHeight> layer;

	while (std::getline(ifs, rowStr)) {
		//std::getline�֐��ŋ�؂蕶�����w�肷��Ƃ��ɕK�v�Ȍ^
		std::istringstream stream(rowStr);//getline�œǂݍ���1�s���R�s�[���Ă�̂�
		std::string columnStr;
		int column = 0;

		//��3������','����؂蕶���Ƃ��A��؂蕶���܂ł𔲂��o���Ă����
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
