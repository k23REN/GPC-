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
  //! �}�E�X�̉�]
  //! SDL���瑊�Α��x�擾
  int x, y;
  Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);
  //! �E�N���b�N���̂�
  if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
    //! �}�E�X�̈ړ��̍ő�l���}500�Ƃ���
    const int maxMouseSpeed = 500;
    //! �ő�ړ��ʂɂ�����p���x
    const float maxOrbitSpeed = Math::Pi * 8;
    float yawSpeed = 0.0f;
    if (x != 0) {
      //! ~[-1.0, 1.0]�͈̔͂ɕϊ�
      yawSpeed = static_cast<float>(x) / maxMouseSpeed;

      yawSpeed *= maxOrbitSpeed;
    }
    m_pCameraComp->SetYawSpeed(-yawSpeed);

  //! �s�b�`���v�Z
    float pitchSpeed = 0.0f;
    if (y != 0) {
      //! ~[-1.0, 1.0]�͈̔͂ɕϊ�
      pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
      pitchSpeed *= maxOrbitSpeed;
    }
    m_pCameraComp->SetPitchSpeed(pitchSpeed);
  }
}

void OrbitActor::SetVisible(bool visible) { m_pMeshComp->SetVisible(visible); }
