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
  // �A�N�^�[�̉�]��������
  Quaternion q = GetRotation();
  // �J�������s�b�`�ɍ��킹�ĉ�]������
  q = Quaternion::Concatenate(q,
                              Quaternion(GetRight(), m_pCameraComp->GetPitch()));
  m_pFPSModel->SetRotation(q);
}

void FPSActor::ActorInput(const uint8_t* keys) {
  float forwardSpeed = 0.0f;
  float strafeSpeed = 0.0f;
  // wasd�L�[�œ���
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

  //! SDL���瑊�Α��x�擾
  int x, y;
  SDL_GetRelativeMouseState(&x, &y);
  //! �}�E�X�̈ړ��̍ő�l���}500�Ƃ���
  const int maxMouseSpeed = 100;
  //! �ő�ړ��ʂɂ�����p���x
  const float maxAngularSpeed = Math::Pi * 8;
  float angularSpeed = 0.0f;
  if (x != 0) {
    //! ~[-1.0, 1.0]�͈̔͂ɕϊ�
    angularSpeed = static_cast<float>(x) / maxMouseSpeed;
    //! �ő�ړ��ʂł̊p���x��������
    angularSpeed *= maxAngularSpeed;
  }
  m_pMoveComp->SetAngularSpeed(angularSpeed);

  //! �s�b�`���v�Z
  const float maxPitchSpeed = Math::Pi * 8;
  float pitchSpeed = 0.0f;
  if (y != 0) {
    //! ~[-1.0, 1.0]�͈̔͂ɕϊ�
    pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
    pitchSpeed *= maxPitchSpeed;
  }
  m_pCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::SetVisible(bool visible) { m_pMeshComp->SetVisible(visible); }
