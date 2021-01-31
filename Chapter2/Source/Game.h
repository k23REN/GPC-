#pragma once
#include "SDL/SDL.h"
#include <unordered_map>
#include <string>
#include <vector>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	
	SDL_Texture* GetTexture(const std::string& fileName);
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	
	std::unordered_map<std::string, SDL_Texture*> m_pTextures;

	std::vector<class Actor*> m_actors;
	std::vector<class Actor*> m_pendingActors;

	std::vector<class SpriteComponent*> m_sprites;

	SDL_Window* mWindow;
	SDL_Renderer* m_pRenderer;
	Uint32 m_ticksCount;
	bool m_isRunning;
	bool m_updatingActors;

	class Ship* m_pShip;
	class Human* m_pHuman;
	class TileMapComponent* m_pTileMap;

	static inline constexpr int m_kWindowWidth = 1024;
	static inline constexpr int m_kWindowHeight = 768;
};
