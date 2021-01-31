#pragma once
#include "SDL/SDL.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Asteroid.h"
#include "Ship.h"
#include "Utility.h"
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

	/**
     * @fn
     * アクターを配列に追加し、一括で処理を行う
     * @brief アクターを追加する
     * @param (a_actor) 追加するアクター
     * @return なし
     */
	void AddActor(Actor* a_actor);

	/**
     * @fn
     * アクターを配列から削除する
     * @brief アクターを削除する
     * @param (a_actor) 削除するアクター
     * @return なし
     */
	void RemoveActor(Actor* a_actor);

	/**
     * @fn
     * スプライトを配列に追加し、一括で処理を行う
     * @brief スプライトを追加する
     * @param (a_sprite) 追加するスプライト
     * @return なし
     */
	void AddSprite(SpriteComponent* a_sprite);

	/**
     * @fn
     * スプライトを配列から削除する
     * @brief スプライトを削除する
     * @param (a_sprite) 削除するスプライト
     * @return なし
     */
	void RemoveSprite(SpriteComponent* a_sprite);
	
	/**
     * @fn
     * アステロイド(小惑星)を配列に追加し、一括で処理を行う
     * @brief アステロイド(小惑星)を追加する
     * @param (a_actor) 追加するアステロイド(小惑星)
     * @return なし
     */
	void AddAsteroid(Asteroid* a_asteroid);

	/**
     * @fn
     * アステロイド(小惑星)を配列から削除する
     * @brief アステロイド(小惑星)を削除する
     * @param (a_actor) 削除するアステロイド(小惑星)
     * @return なし
     */
    void RemoveAsteroid(Asteroid* a_asteroid);

	std::vector<Asteroid*>& GetAsteroids() { return m_asteroids; }

	[[nodiscard]] SDL_Texture* GetTexture(const std::string& fileName);

	static inline constexpr int m_kWindowWidth = 1024;
	static inline constexpr int m_kWindowHeight = 768;

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	
	std::unordered_map<std::string, SDL_Texture*> m_pTextures;

	std::vector<Actor*> m_actors;
	std::vector<Actor*> m_pendingActors;

	std::vector<SpriteComponent*> m_sprites;

	///
	///SDL系変数
	///
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* m_pRenderer = nullptr;

	Uint32 m_ticksCount = 0;
	bool m_isRunning = true;
	bool m_updatingActors = false;
	
	Sptr<Ship> m_pShip = nullptr;
    std::vector<Asteroid*> m_asteroids;

};
