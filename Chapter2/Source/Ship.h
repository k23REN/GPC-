#pragma once
#include "Actor.h"
class Ship : public Actor
{
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime) override final;
	void ProcessKeyboard(const uint8_t* state);

	float GetRightSpeed() const { return m_rightSpeed; }
	float GetDownSpeed() const { return m_downSpeed; }
private:
	float m_rightSpeed;
	float m_downSpeed;
};