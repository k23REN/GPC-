#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component {
 public:
  MoveComponent(Actor* owner, int updateOrder = 10);

  void Update(float deltaTime) override final;
  void ProcessInput(const uint8_t* keyState) override{};

  /**
   * @fn
   * ������͂����v������
   * @brief �͂�������
   * @param (a_force) ������͂̑傫��
   * @return �Ȃ�
   */
  void AddForce(const Vector3& a_force);

  ///
  ///�擾
  ///
  [[nodiscard]] const float GetAngularSpeed() const { return m_angularSpeed; }
  [[nodiscard]] const float GetForwardSpeed() const { return m_forwardSpeed; }
  [[nodiscard]] const float GetStrafeSpeed() const { return m_strafeSpeed; }

  ///
  ///�ݒ�
  ///
  void SetAngularSpeed(float a_speed) { m_angularSpeed = a_speed; }
  void SetForwardSpeed(float a_speed) { m_forwardSpeed = a_speed; }
  void SetStrafeSpeed(float a_speed)  { m_strafeSpeed = a_speed; }
  void SetMass(float a_mass) { m_mass = a_mass; }
  void SetMaxSpeed(float a_maxSpeed) { m_maxSpeed = a_maxSpeed; }

  void ClearSpeed() { m_velocity = Vector3::Zero; }

 private:
  //!��]����
  float m_angularSpeed = 0.0f;
  //!�O�i�^������
  float m_forwardSpeed = 0.0f;
  //!�X�g���C�h���x
  float m_strafeSpeed = 0.0f;

  //!����
  float m_mass = 0.1f;
  //!�͂̍��v
  Vector3 m_sumOfForces = Vector3::Zero;
  //!���x
  Vector3 m_velocity = Vector3::Zero;
  //!�����x
  Vector3 m_acceleration;

  float m_maxSpeed = 300.0f;
};