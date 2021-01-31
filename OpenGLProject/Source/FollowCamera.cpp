#include "FollowCamera.h"

#include "Actor.h"

FollowCamera::FollowCamera(Actor* owner)
    : CameraComponent(owner),
      m_horzDist(350.0f),
      m_vertDist(150.0f),
      m_targetDist(100.0f),
      m_springConstant(64.0f),
  
      m_offset(-400.0f, 0.0f, 0.0f),
      m_up(Vector3::UnitZ),
      m_pitchSpeed(0.0f),
      m_yawSpeed(0.0f) {}

void FollowCamera::Update(float deltaTime) {
  CameraComponent::Update(deltaTime);
  //! �΂˒萔���猸�����v�Z
  float dampening = 2.0f * Math::Sqrt(m_springConstant);
  //! �ړI�ʒu���v�Z
  Vector3 idealPos = ComputeCameraPos();
  //! ���݈ʒu�ƖړI��̍����v�Z
  Vector3 diff = m_actualPos - idealPos;
  //! �΂˂ɂ������x���v�Z
  Vector3 acel = -m_springConstant * diff - dampening * m_velocity;

  //! ���x���v�Z
  m_velocity += acel * deltaTime;
  //! �J�����̍��W���X�V
  m_actualPos += m_velocity * deltaTime;
  //! �^�[�Q�b�g�̓A�N�^�[����O���ɗ��ꂽ���W�ɐݒ�
  Vector3 target = m_pOwner->GetPosition() + m_pOwner->GetForward() * m_targetDist;

  Vector3 cameraPos;

  //! �s�b�`�ƃ��[
  {
    //! ���[���h��y���������郈�[�̃N�H�[�^�j�I�����쐬
    Quaternion yaw(Vector3::UnitZ, m_yawSpeed * deltaTime);
    //! �J�����̃I�t�Z�b�g�Ə���x�N�g�������[�ŕϊ�
    m_offset = Vector3::Transform(m_offset, yaw);
    m_up = Vector3::Transform(m_up, yaw);

    //! �J�����̑O���ƉE�����v�Z����
    //! owner.position - (owner.position + offset) = -offset
    Vector3 forward = -1.0f * m_offset;
    forward.Normalize();
    Vector3 right = Vector3::Cross(m_up, forward);
    right.Normalize();

    //! �J�����̉E�������Ƃ���s�b�`�̃N�H�[�^�j�I�����쐬
    Quaternion pitch(right, m_pitchSpeed * deltaTime);
    //! �J�����̃I�t�Z�b�g�Ə���x�N�g�����s�b�`�ŕϊ�
    m_offset = Vector3::Transform(m_offset, pitch);
    m_up = Vector3::Transform(m_up, pitch);
    cameraPos = m_actualPos + m_offset;
  }

  // Matrix4 view = Matrix4::CreateLookAt(cameraPos, m_actualPos, m_up);
  Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, m_up);
  SetViewMatrix(view);
}

void FollowCamera::SnapToIdeal() {
  //! ���ۂ̈ʒu�͗��z�̈ʒu�Ɠ����ɂ���
  m_actualPos = ComputeCameraPos();
  // ���x���[���ɏ�����
  m_velocity = Vector3::Zero;
  // �����_�ƃr���[���v�Z
  Vector3 target =
      m_pOwner->GetPosition() + m_pOwner->GetForward() * m_targetDist;

  Matrix4 view = Matrix4::CreateLookAt(m_actualPos, target, Vector3::UnitZ);
  SetViewMatrix(view);
}

Vector3 FollowCamera::ComputeCameraPos() const {
  //! �J�����̈ʒu���A�N�^�[�̏����ɃZ�b�g
  Vector3 cameraPos = m_pOwner->GetPosition();
  cameraPos += Vector3::UnitZ * m_vertDist;
  return cameraPos;
}
