#pragma once
#include "Actor.h"
#include "OrbitCamera.h"
#include "MeshComponent.h"

class OrbitActor : public Actor
{
public:
	OrbitActor(class Game* game);

	void ActorInput(const uint8_t* keys) override;

	void SetVisible(bool visible);
private:
	OrbitCamera* m_pCameraComp;
	MeshComponent* m_pMeshComp;
};
