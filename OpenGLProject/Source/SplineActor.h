#pragma once
#include "Actor.h"
#include "SplineCamera.h"

class SplineActor : public Actor
{
public:
	SplineActor(class Game* a_game);

	void ActorInput(const uint8_t* a_keys) override;

	void RestartSpline();
private:
	SplineCamera* m_pCameraComp;
};
