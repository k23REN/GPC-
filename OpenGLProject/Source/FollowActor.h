#pragma once
#include "Actor.h"
#include "FollowCamera.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "SkeletalMeshComponent.h"

class FollowActor : public Actor {
 public:
  FollowActor(class Game* game);

  void ActorInput(const uint8_t* keys) override;

  void SetVisible(bool visible);

 private:
  MoveComponent* m_pMoveComp;
  FollowCamera* m_pCameraComp;
  SkeletalMeshComponent* m_pMeshComp;
  bool m_moving = false;
};
