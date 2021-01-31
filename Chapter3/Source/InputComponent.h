#pragma once
#include <cstdint>

#include "MoveComponent.h"

class InputComponent : public MoveComponent {
 public:
  InputComponent(Actor* owner);

  void ProcessInput(const uint8_t* keyState) override final;

  ///
  ///éÊìæ
  ///
  float GetMaxForward() const { return m_maxForwardSpeed; }
  float GetMaxAngular() const { return m_maxAngularSpeed; }
  int GetForwardKey() const { return m_forwardKey; }
  int GetBackKey() const { return m_backKey; }
  int GetClockwiseKey() const { return m_clockwiseKey; }
  int GetCounterClockwiseKey() const { return m_counterClockwiseKey; }

  ///
  ///ê›íË
  ///
  void SetMaxForwardSpeed(float speed) { m_maxForwardSpeed = speed; }
  void SetMaxAngularSpeed(float speed) { m_maxAngularSpeed = speed; }
  void SetForwardKey(int key) { m_forwardKey = key; }
  void SetBackKey(int key) { m_backKey = key; }
  void SetClockwiseKey(int key) { m_clockwiseKey = key; }
  void SetCounterClockwiseKey(int key) { m_counterClockwiseKey = key; }

 private:
  float m_maxForwardSpeed = 0.0f;
  float m_maxAngularSpeed = 0.0f;

  int m_forwardKey = 0;
  int m_backKey = 0;

  int m_clockwiseKey = 0;
  int m_counterClockwiseKey = 0;
};
