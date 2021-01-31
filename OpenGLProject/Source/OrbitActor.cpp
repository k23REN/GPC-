#include "OrbitActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MoveComponent.h"

OrbitActor::OrbitActor(Game* game) : Actor(game) {
  m_pMeshComp = new MeshComponent(
      this, game->GetRenderer()->GetMesh("../Assets/RacingCar.gpmesh"));
  SetPosition(Vector3(0.0f, 0.0f, -100.0f));

  m_pCameraComp = new OrbitCamera(this);
}

void OrbitActor::ActorInput(const uint8_t* keys) {
  //! マウスの回転
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
  }
}

void OrbitActor::SetVisible(bool visible) { m_pMeshComp->SetVisible(visible); }
