#pragma once
#include "Actor.h"
#include "CircleComponent.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"

class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);

	~Asteroid();

	void UpdateActor(float a_deltaTime) override final;

	[[nodiscard]] Sptr<CircleComponent> GetCircle() { return m_pCircle; }

private:
	Sptr<CircleComponent> m_pCircle;
	Sptr<SpriteComponent> m_pSprite;
	Sptr<MoveComponent> m_pMove;

	static inline constexpr float m_kSpeed = 150.0f;
    static inline constexpr float m_kRadius = 40.0f;
};