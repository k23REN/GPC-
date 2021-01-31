#pragma once
#include "Actor.h"
#include "CircleComponent.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
class Laser : public Actor 
{
 public:
  Laser(class Game* game);

  void UpdateActor(float deltaTime) override final;

 private:
  Sptr<CircleComponent> m_pCircle;
  float m_deathTimer;

  Sptr<SpriteComponent> m_pSprite;
  Sptr<MoveComponent> m_pMove;

  static inline constexpr float m_kSpeed = 800.0f;
  static inline constexpr float m_kRadius = 11.0f;
};
