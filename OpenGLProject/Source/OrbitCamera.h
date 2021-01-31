#pragma once
#include "CameraComponent.h"

class OrbitCamera : public CameraComponent {
 public:
  OrbitCamera(class Actor* owner);

  void Update(float deltaTime) override;

  ///
  /// 取得
  /// 
  [[nodiscard]] const float GetPitchSpeed() const { return m_pitchSpeed; }
  [[nodiscard]] const float GetYawSpeed() const { return m_yawSpeed; }

  ///
  /// 設定
  /// 
  void SetPitchSpeed(float speed) { m_pitchSpeed = speed; }
  void SetYawSpeed(float speed) { m_yawSpeed = speed; }

 private:
  // ターゲットからのオフセット
  Vector3 m_offset;
  // カメラの上方ベクトル
  Vector3 m_up;
  // ピッチの角速度
  float m_pitchSpeed;
  // ヨーの角速度
  float m_yawSpeed;
};
