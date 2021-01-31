#pragma once
#include "CameraComponent.h"

class OrbitCamera : public CameraComponent {
 public:
  OrbitCamera(class Actor* owner);

  void Update(float deltaTime) override;

  ///
  /// �擾
  /// 
  [[nodiscard]] const float GetPitchSpeed() const { return m_pitchSpeed; }
  [[nodiscard]] const float GetYawSpeed() const { return m_yawSpeed; }

  ///
  /// �ݒ�
  /// 
  void SetPitchSpeed(float speed) { m_pitchSpeed = speed; }
  void SetYawSpeed(float speed) { m_yawSpeed = speed; }

 private:
  // �^�[�Q�b�g����̃I�t�Z�b�g
  Vector3 m_offset;
  // �J�����̏���x�N�g��
  Vector3 m_up;
  // �s�b�`�̊p���x
  float m_pitchSpeed;
  // ���[�̊p���x
  float m_yawSpeed;
};
