#include "FPSActor.h"

#include "Game.h"
#include "Renderer.h"
#include "SDL/SDL_scancode.h"

FPSActor::FPSActor(Game* game) : Actor(game) {
  m_pMoveComp = std::make_shared<MoveComponent>(this);
  m_lastFootstep = 0.0f;

  m_pCameraComp = std::make_shared<FPSCamera>(this);

  m_pFPSModel = new Actor(game);
  m_pFPSModel->SetScale(0.75f);
  m_pMeshComp = new MeshComponent(
      m_pFPSModel, game->GetRenderer()->GetMesh("../Assets/Rifle.gpmesh"));
}

void FPSActor::UpdateActor(float deltaTime) {
  Actor::UpdateActor(deltaTime);

  m_lastFootstep -= deltaTime;
  if (!Math::NearZero(m_pMoveComp->GetForwardSpeed()) && m_lastFootstep <= 0.0f) {
    m_lastFootstep = 0.5f;
  }

  const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
  Vector3 modelPos = GetPosition();
  modelPos += GetForward() * modelOffset.x;
  modelPos += GetRight() * modelOffset.y;
  modelPos.z += modelOffset.z;
  m_pFPSModel->SetPosition(modelPos);
  // アクターの回転を初期化
  Quaternion q = GetRotation();
  // カメラをピッチに合わせて回転させる
  q = Quaternion::Concatenate(q,
                              Quaternion(GetRight(), m_pCameraComp->GetPitch()));
  m_pFPSModel->SetRotation(q);
}

void FPSActor::ActorInput(const uint8_t* keys) {
  float forwardSpeed = 0.0f;
  float strafeSpeed = 0.0f;
  // wasdキーで動く
  if (keys[SDL_SCANCODE_W]) {
    forwardSpeed += 400.0f;
  }
  if (keys[SDL_SCANCODE_S]) {
    forwardSpeed -= 400.0f;
  }
  if (keys[SDL_SCANCODE_A]) {
    strafeSpeed -= 400.0f;
  }
  if (keys[SDL_SCANCODE_D]) {
    strafeSpeed += 400.0f;
  }

  m_pMoveComp->SetForwardSpeed(forwardSpeed);
  m_pMoveComp->SetStrafeSpeed(strafeSpeed);

  //! SDLから相対速度取得
  int x, y;
  SDL_GetRelativeMouseState(&x, &y);
  //! マウスの移動の最大値を±500とする
  const int maxMouseSpeed = 100;
  //! 最大移動量における角速度
  const float maxAngularSpeed = Math::Pi * 8;
  float angularSpeed = 0.0f;
  if (x != 0) {
    //! ~[-1.0, 1.0]の範囲に変換
    angularSpeed = static_cast<float>(x) / maxMouseSpeed;
    //! 最大移動量での角速度をかける
    angularSpeed *= maxAngularSpeed;
  }
  m_pMoveComp->SetAngularSpeed(angularSpeed);

  //! ピッチを計算
  const float maxPitchSpeed = Math::Pi * 8;
  float pitchSpeed = 0.0f;
  if (y != 0) {
    //! ~[-1.0, 1.0]の範囲に変換
    pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
    pitchSpeed *= maxPitchSpeed;
  }
  m_pCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::SetVisible(bool visible) { m_pMeshComp->SetVisible(visible); }
