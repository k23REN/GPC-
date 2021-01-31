#include "Laser.h"

#include "Asteroid.h"
#include "CircleComponent.h"
#include "Game.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"

Laser::Laser(Game* game) 
    : Actor(game), m_deathTimer(1.0f) {
  m_pSprite = std::make_shared<SpriteComponent>(this);
  m_pSprite->SetTexture(game->GetTexture("../Assets/Laser.png"));

  m_pMove = std::make_shared<MoveComponent>(this);
  Vector2 force = Vector2(400.0f, 400.0f);
  m_pMove->AddForce(force);
  m_pMove->SetMaxSpeed(800.0f);

  m_pCircle = std::make_shared<CircleComponent>(this);
  m_pCircle->SetRadius(m_kRadius);
}

void Laser::UpdateActor(float deltaTime) 
{
  Vector2 force = Vector2(400.0f, 400.0f);
  m_pMove->AddForce(force);

  m_deathTimer -= deltaTime;
  if (m_deathTimer <= 0.0f) {
    SetState(eDead);
  } else {
    for (auto ast : GetGame()->GetAsteroids()) {
      if (Intersect(*m_pCircle, *(ast->GetCircle()))) {  //‚à‚µ“–‚½‚Á‚Ä‚¢‚ê‚Î
        SetState(eDead);
        ast->SetState(eDead);
        break;
      }
    }
  }
}