#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "Game.h"
#include "Laser.h"
#include "InputComponent.h"

Ship::Ship(Game* game)
	:Actor(game)
{
      m_pSprite = std::make_shared<SpriteComponent>(this);
        m_pSprite->SetTexture(game->GetTexture("../Assets/Ship.png"));

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
        m_position = Vector2(0.0f, 0.0f);
        m_recomputeWorldTransform = true;
        break;
      }
    }

    if (m_state == eInvisible) {
      m_deathTimer -= deltaTime;

      if (m_deathTimer <= 0) {
        SetState(eActive);
        m_deathTimer = 1.0f;
        m_pInput->ClearSpeed();
      }
    }
}

void Ship::ActorInput(const uint8_t* keyState) 
{
  if (keyState[SDL_SCANCODE_SPACE] && m_laserCooldown <= 0.0f) {
    Laser* laser = new Laser(GetGame());
    laser->SetPosition(m_position);
    laser->SetRotation(m_rotation);

    // クールダウンをリセット
    m_laserCooldown = 0.5f;
  }
}
