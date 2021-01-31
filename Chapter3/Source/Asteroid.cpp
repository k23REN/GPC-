#include "Asteroid.h"
#include "Game.h"
#include "Random.h"
Asteroid::Asteroid(Game* game) : Actor(game) 
{
  Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f));
  SetPosition(randPos);

  SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

  m_pSprite = std::make_shared<SpriteComponent>(this);
  m_pSprite->SetTexture(game->GetTexture("../Assets/Asteroid.png"));

  m_pMove = std::make_shared<MoveComponent>(this);
  Vector2 force = Vector2(10.0f, 10.0f);
  m_pMove->AddForce(force);

  m_pCircle = std::make_shared<CircleComponent>(this);
  m_pCircle->SetRadius(m_kRadius);

  game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this); }

void Asteroid::UpdateActor(float a_deltaTime) 
{
  Vector2 force = Vector2(10.0f, 10.0f);
  m_pMove->AddForce(force);
}
