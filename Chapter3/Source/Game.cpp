#include "Game.h"
#include "SDL/SDL_image.h"
#include <algorithm>
#include "BGSpriteComponent.h"

Game::Game()
{

}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("SDL Initialize Failed : %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Chapter3", 100, 100, m_kWindowWidth, m_kWindowHeight, 0);
	if (!mWindow) {
		SDL_Log("CreateWindow Failed: %s", SDL_GetError());
		return false;
	}

	m_pRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_pRenderer) {
		SDL_Log("CreateRenderer Failed : %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("SDL_image Initialize Failed : %s", SDL_GetError());
		return false;
	}

	LoadData();

	m_ticksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (m_isRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_isRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		m_isRunning = false;
	}

	m_updatingActors = true;
    for (auto actor : m_actors) {
      actor->ProcessInput(state);
    }
    m_updatingActors = false;
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - m_ticksCount) / 1000.0f;
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	m_ticksCount = SDL_GetTicks();

	m_updatingActors = true;
	for (auto actor : m_actors) {
		actor->Update(deltaTime);
	}
	m_updatingActors = false;

	for (auto pending : m_pendingActors) {
		m_actors.emplace_back(pending);
	}
	m_pendingActors.clear();

	std::vector<Actor*> deadActors;
	for (auto actor : m_actors) {
		if (actor->GetState() == Actor::eDead) {
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors) {
		delete actor;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);

	for (auto sprite : m_sprites) {
 		sprite->Draw(m_pRenderer);
	}

	SDL_RenderPresent(m_pRenderer);
}

void Game::LoadData()
{

	m_pShip = std::make_shared<Ship>(this);
	m_pShip->SetPosition(Vector2(100.0f, m_kWindowHeight / 2.0f));
	m_pShip->SetScale(1.5f);

	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(m_kWindowWidth / 2.0f, m_kWindowHeight / 2.0f));

	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(m_kWindowWidth, m_kWindowHeight));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("../Assets/Farback01.png"),
		GetTexture("../Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);

	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(m_kWindowWidth, m_kWindowHeight));
	bgtexs = {
		GetTexture("../Assets/Stars.png"),
		GetTexture("../Assets/Stars.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);

	constexpr int kNumAsteroids = 20;
    for (int i = 0; i < kNumAsteroids; i++) {
      new Asteroid(this);
    }
}

void Game::UnloadData()
{
	for (auto i : m_pTextures) {
		SDL_DestroyTexture(i.second);
	}
	m_pTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* tex = nullptr;
	auto iter = m_pTextures.find(fileName);
	if (iter != m_pTextures.end()) {
		tex = iter->second;
	}
	else {
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf) {
			SDL_Log("Failed to load texture file %s", fileName.c_str());
            assert(FALSE);
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(m_pRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex) {
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
            assert(FALSE);
			return nullptr;
		}

		m_pTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	if (m_updatingActors) {
		m_pendingActors.emplace_back(actor);
	}
	else {
		m_actors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
  auto iter = std::find(begin(m_pendingActors), end(m_pendingActors), actor);
  if (iter != end(m_pendingActors)) {
    std::iter_swap(iter, end(m_pendingActors) - 1);
		m_pendingActors.pop_back();
	}

	iter = std::find(begin(m_actors), end(m_actors), actor);
        if (iter != end(m_actors)) {
          std::iter_swap(iter, end(m_actors) - 1);
		m_actors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = m_sprites.begin();
	for (;
		 iter != m_sprites.end();
		 ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}

	m_sprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(m_sprites.begin(), m_sprites.end(), sprite);
	m_sprites.erase(iter);
}

void Game::AddAsteroid(Asteroid* a_asteroid) 
{
	m_asteroids.emplace_back(a_asteroid);
}

void Game::RemoveAsteroid(Asteroid* a_asteroid) 
{
  auto it = std::find(m_asteroids.begin(), m_asteroids.end(), a_asteroid);

  if (it != m_asteroids.end()) {
    m_asteroids.erase(it);
  }
}