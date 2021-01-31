#include "SplineActor.h"

#include "Game.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Renderer.h"

SplineActor::SplineActor(Game* a_game) : Actor(a_game) {
  //MeshComponent* mc = new MeshComponent(
  //    this, a_game->GetRenderer()->GetMesh("../Assets/RacingCar.gpmesh"));
  //SetPosition(Vector3(0.0f, 0.0f, -100.0f));

  m_pCameraComp = new SplineCamera(this);

  //! ƒXƒvƒ‰ƒCƒ“¶¬
  Spline path;
  path.m_controlPoints.emplace_back(Vector3::Zero);
  for (int i = 0; i < 5; i++) {
    if (i % 2 == 0) {
      path.m_controlPoints.emplace_back(Vector3(300.0f * (i + 1), 300.0f, 300.0f));
    } else {
      path.m_controlPoints.emplace_back(Vector3(300.0f * (i + 1), 0.0f, 0.0f));
    }
  }

  m_pCameraComp->SetSpline(path);
  m_pCameraComp->SetPaused(false);
}

void SplineActor::ActorInput(const uint8_t* a_keys) {}

void SplineActor::RestartSpline() { m_pCameraComp->Restart(); }
