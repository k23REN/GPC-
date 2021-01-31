#include "Human.h"
#include "Game.h"

Human::Human(Game* game)
	:Actor(game)
	, m_rightSpeed(0.0f)
	, m_downSpeed(0.0f)
	, m_asc(nullptr)
{
	m_asc = new AnimSpriteComponent(this);

	AnimInfo animInfo;
	animInfo.m_animTextures = {
		game->GetTexture("../Assets/Character01.png"),
		game->GetTexture("../Assets/Character02.png"),
		game->GetTexture("../Assets/Character03.png"),
		game->GetTexture("../Assets/Character04.png"),
		game->GetTexture("../Assets/Character05.png"),
		game->GetTexture("../Assets/Character06.png"),
	};
	animInfo.m_isLoop = true;

	m_asc->SetAnimTextures("Walk", animInfo);

	animInfo.m_animTextures = {
	game->GetTexture("../Assets/Character07.png"),
	game->GetTexture("../Assets/Character08.png"),
	game->GetTexture("../Assets/Character09.png"),
	game->GetTexture("../Assets/Character10.png"),
	game->GetTexture("../Assets/Character11.png"),
	game->GetTexture("../Assets/Character12.png"),
	game->GetTexture("../Assets/Character13.png"),
	game->GetTexture("../Assets/Character14.png"),
	game->GetTexture("../Assets/Character15.png"),
	};
	animInfo.m_isLoop = false;

	m_asc->SetAnimTextures("Jump", animInfo);

	animInfo.m_animTextures = {
		game->GetTexture("../Assets/Character16.png"),
		game->GetTexture("../Assets/Character17.png"),
		game->GetTexture("../Assets/Character18.png"),
	};
	animInfo.m_isLoop = false;

	m_asc->SetAnimTextures("Punch", animInfo);

	m_asc->SetPlayAnim("Walk");
}

void Human::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
}

void Human::ProcessKeyboard(const uint8_t* state)
{
	if (state[SDL_SCANCODE_1]) {
		m_asc->SetPlayAnim("Walk");
	}
	if (state[SDL_SCANCODE_2]) {
		m_asc->SetPlayAnim("Punch");
	}
	if (state[SDL_SCANCODE_3]) {
		m_asc->SetPlayAnim("Jump");
	}
}
