#pragma once
#include "Actor.h"
#include "MoveComponent.h"

class CameraActor : public Actor
{
public:
	CameraActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;
private:
	Sptr<MoveComponent> m_pMoveComp;
};
