#pragma once
#include "Actor.h"
#include "InputComponent.h"
#include "CircleComponent.h"
#include "SpriteComponent.h"
class Ship : public Actor
{
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime) override final;
    void ActorInput(const uint8_t* keyState) override final;

private:
    float m_laserCooldown;
    Sptr<InputComponent> m_pInput;
    Sptr<CircleComponent> m_pCircle;
    float m_deathTimer = 1.0f;

    Sptr<SpriteComponent> m_pSprite;


    static inline constexpr float m_kSpeed = 300.0f;
    static inline constexpr float m_kRadius = 30.0f;
};