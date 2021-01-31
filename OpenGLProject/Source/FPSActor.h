#pragma once
#include "Actor.h"
#include "MoveComponent.h"
#include "MeshComponent.h"
#include "FPSCamera.h"

class FPSActor : public Actor
{
public:
	FPSActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;

	void SetVisible(bool visible);

private:
	Sptr<MoveComponent> m_pMoveComp;
	MeshComponent* m_pMeshComp;
	Sptr<FPSCamera> m_pCameraComp;
	Actor* m_pFPSModel;
	float m_lastFootstep;
};