#pragma once
#include "CameraComponent.h"

class FollowCamera : public CameraComponent {
 public:
  FollowCamera(class Actor* owner);

  void Update(float deltaTime) override;

  void SnapToIdeal();

  ///
  /// �擾
  /// 
  [[nodiscard]] const float GetPitchSpeed() const { return m_pitchSpeed; }
  [[nodiscard]] const float GetYawSpeed() const { return m_yawSpeed; }

  /// 
  /// �ݒ�
  /// 
  void SetHorzDist(float dist) { m_horzDist = dist; }
  void SetVertDist(float dist) { m_vertDist = dist; }
  void SetTargetDist(float dist) { m_targetDist = dist; }
  void SetSpringConstant(float spring) { m_springConstant = spring; }
  void SetPitchSpeed(float speed) { m_pitchSpeed = speed; }
  void SetYawSpeed(float speed) { m_yawSpeed = speed; }

 private:
  Vector3 ComputeCameraPos() const;

  //! ���ۂ̃J�����̈ʒu
  Vector3 m_actualPos;
  //! ���ۂ̃J�����̑��x
  Vector3 m_velocity;
  //! �����̒Ǐ]���鋗��
  float m_horzDist;
  //! �����̒Ǐ]���鋗��
  float m_vertDist;
  //! �^�[�Q�b�g�܂ł̋���
  float m_targetDist;
  //! �΂˂̒萔 (�����قǌł��Ȃ�)
  float m_springConstant;

  // �^�[�Q�b�g����̃I�t�Z�b�g
  Vector3 m_offset;
  // �J�����̏���x�N�g��
  Vector3 m_up;
  // �s�b�`�̊p���x
  float m_pitchSpeed;
  // ���[�̊p���x
  float m_yawSpeed;
};
