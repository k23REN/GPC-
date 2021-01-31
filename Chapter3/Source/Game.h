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
     * �A�N�^�[��z��ɒǉ����A�ꊇ�ŏ������s��
     * @brief �A�N�^�[��ǉ�����
     * @param (a_actor) �ǉ�����A�N�^�[
     * @return �Ȃ�
     */
	void AddActor(Actor* a_actor);

	/**
     * @fn
     * �A�N�^�[��z�񂩂�폜����
     * @brief �A�N�^�[���폜����
     * @param (a_actor) �폜����A�N�^�[
     * @return �Ȃ�
     */
	void RemoveActor(Actor* a_actor);

	/**
     * @fn
     * �X�v���C�g��z��ɒǉ����A�ꊇ�ŏ������s��
     * @brief �X�v���C�g��ǉ�����
     * @param (a_sprite) �ǉ�����X�v���C�g
     * @return �Ȃ�
     */
	void AddSprite(SpriteComponent* a_sprite);

	/**
     * @fn
     * �X�v���C�g��z�񂩂�폜����
     * @brief �X�v���C�g���폜����
     * @param (a_sprite) �폜����X�v���C�g
     * @return �Ȃ�
     */
	void RemoveSprite(SpriteComponent* a_sprite);
	
	/**
     * @fn
     * �A�X�e���C�h(���f��)��z��ɒǉ����A�ꊇ�ŏ������s��
     * @brief �A�X�e���C�h(���f��)��ǉ�����
     * @param (a_actor) �ǉ�����A�X�e���C�h(���f��)
     * @return �Ȃ�
     */
	void AddAsteroid(Asteroid* a_asteroid);

	/**
     * @fn
     * �A�X�e���C�h(���f��)��z�񂩂�폜����
     * @brief �A�X�e���C�h(���f��)���폜����
     * @param (a_actor) �폜����A�X�e���C�h(���f��)
     * @return �Ȃ�
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
	///SDL�n�ϐ�
	///
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* m_pRenderer = nullptr;

	Uint32 m_ticksCount = 0;
	bool m_isRunning = true;
	bool m_updatingActors = false;
	
	Sptr<Ship> m_pShip = nullptr;
    std::vector<Asteroid*> m_asteroids;

};
