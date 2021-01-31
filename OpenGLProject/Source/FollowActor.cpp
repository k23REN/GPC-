#include "FollowActor.h"

#include "Game.h"
#include "Renderer.h"

FollowActor::FollowActor(Game* game) : Actor(game) {
  m_pMeshComp = new MeshComponent(
      this, game->GetRenderer()->GetMesh("../Assets/RacingCar.gpmesh"));

  SetPosition(Vector3(0.0f, 0.0f, -100.0f));

  m_pMoveComp = new MoveComponent(this);
  m_pCameraComp = new FollowCamera(this);
  m_pCameraComp->SnapToIdeal();
}

void FollowActor::ActorInput(const uint8_t* keys) {
  float forwardSpeed = 0.0f;
  float angularSpeed = 0.0f;
  // wasdキーで動かす
  if (keys[SDL_SCANCODE_W]) {
    forwardSpeed += 400.0f;
  }
  if (keys[SDL_SCANCODE_S]) {
    forwardSpeed -= 400.0f;
  }
  if (keys[SDL_SCANCODE_A]) {
    angularSpeed -= Math::Pi;
  }
  if (keys[SDL_SCANCODE_D]) {
    angularSpeed += Math::Pi;
  }

  //! マウスの動き
  {
    //! SDLから相対速度取得
    int x, y;
    Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);
    //! 右クリック時のみ
    if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
      //! マウスの移動の最大値を±500とする
      const int maxMouseSpeed = 500;
      //! 最大移動量における角速度
      const float maxOrbitSpeed = Math::Pi * 8;
      float yawSpeed = 0.0f;
      if (x != 0) {
        //! ~[-1.0, 1.0]の範囲に変換
        yawSpeed = static_cast<float>(x) / maxMouseSpeed;

        yawSpeed *= maxOrbitSpeed;
      }
      m_pCameraComp->SetYawSpeed(-yawSpeed);

      //! ピッチを計算
      float pitchSpeed = 0.0f;
      if (y != 0) {
        //! ~[-1.0, 1.0]の範囲に変換
        pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
        pitchSpeed *= maxOrbitSpeed;
      }
      m_pCameraComp->SetPitchSpeed(pitchSpeed);
    } else {
      m_pCameraComp->SetYawSpeed(0.0f);
      m_pCameraComp->SetPitchSpeed(0.0f);

    }
  }

  m_pMoveComp->SetForwardSpeed(forwardSpeed);
  m_pMoveComp->SetAngularSpeed(angularSpeed);

  // 速度に基づいて水平位置の変更
  if (!Math::NearZero(forwardSpeed)) {
    m_pCameraComp->SetHorzDist(500.0f);
  } else {
    m_pCameraComp->SetHorzDist(350.0f);
  }
}

void FollowActor::SetVisible(bool visible) { m_pMeshComp->SetVisible(visible); }
