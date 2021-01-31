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
  // wasd�L�[�œ�����
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

  //! �}�E�X�̓���
  {
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
    } else {
      m_pCameraComp->SetYawSpeed(0.0f);
      m_pCameraComp->SetPitchSpeed(0.0f);

    }
  }

  m_pMoveComp->SetForwardSpeed(forwardSpeed);
  m_pMoveComp->SetAngularSpeed(angularSpeed);

  // ���x�Ɋ�Â��Đ����ʒu�̕ύX
  if (!Math::NearZero(forwardSpeed)) {
    m_pCameraComp->SetHorzDist(500.0f);
  } else {
    m_pCameraComp->SetHorzDist(350.0f);
  }
}

void FollowActor::SetVisible(bool visible) { m_pMeshComp->SetVisible(visible); }
