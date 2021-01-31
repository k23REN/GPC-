#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
#include "Laser.h"
#include "InputComponent.h"

Ship::Ship(Game* game)
	:Actor(game)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);

	AnimInfo animInfo;
	animInfo.m_animTextures = {
		game->GetTexture("../Assets/Ship01.png"),
		game->GetTexture("../Assets/Ship02.png"),
		game->GetTexture("../Assets/Ship03.png"),
		game->GetTexture("../Assets/Ship04.png"),
	};

	animInfo.m_isLoop = true;
	asc->SetAnimTextures("Fire", animInfo);

	asc->SetPlayAnim("Fire");

    // 入力情報設定
	{
        m_pInput = std::make_shared<InputComponent>(this);
        m_pInput->SetForwardKey(SDL_SCANCODE_W);
        m_pInput->SetBackKey(SDL_SCANCODE_S);
        m_pInput->SetClockwiseKey(SDL_SCANCODE_A);
        m_pInput->SetCounterClockwiseKey(SDL_SCANCODE_D);
        m_pInput->SetMaxForwardSpeed(m_kSpeed);
        m_pInput->SetMaxAngularSpeed(Math::TwoPi);
        AddComponent(m_pInput.get());
    }

    m_pCircle = std::make_shared<CircleComponent>(this);
    m_pCircle->SetRadius(m_kRadius);
}

void Ship::UpdateActor(float deltaTime) {
    m_laserCooldown -= deltaTime;

    for (auto ast : GetGame()->GetAsteroids()) {
      if (Intersect(*m_pCircle,
                    *(ast->GetCircle()))) {  //もし当たっていれば
        SetState(eInvisible);
        m_position = Vector2(Game::m_kWindowWidth * 0.5f, Game::m_kWindowHeight * 0.5f);
        break;
      }
    }

    if (m_state == eInvisible) {
      m_deathTimer -= deltaTime;

      if (m_deathTimer <= 0) {
        SetState(eActive);
        m_deathTimer = 1.0f;
      }
    }
}

void Ship::ActorInput(const uint8_t* keyState) 
{
  if (keyState[SDL_SCANCODE_SPACE] && m_laserCooldown <= 0.0f) {
    Laser* laser = new Laser(GetGame());
    laser->SetPosition(GetPosition());
    laser->SetRotation(GetRotation());

    // クールダウンをリセット
    m_laserCooldown = 0.5f;
  }
}
