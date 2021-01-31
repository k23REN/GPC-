#pragma once
#include "CameraComponent.h"

class FollowCamera : public CameraComponent {
 public:
  FollowCamera(class Actor* owner);

  void Update(float deltaTime) override;

  void SnapToIdeal();

  ///
  /// 取得
  /// 
  [[nodiscard]] const float GetPitchSpeed() const { return m_pitchSpeed; }
  [[nodiscard]] const float GetYawSpeed() const { return m_yawSpeed; }

  /// 
  /// 設定
  /// 
  void SetHorzDist(float dist) { m_horzDist = dist; }
  void SetVertDist(float dist) { m_vertDist = dist; }
  void SetTargetDist(float dist) { m_targetDist = dist; }
  void SetSpringConstant(float spring) { m_springConstant = spring; }
  void SetPitchSpeed(float speed) { m_pitchSpeed = speed; }
  void SetYawSpeed(float speed) { m_yawSpeed = speed; }

 private:
  Vector3 ComputeCameraPos() const;

  //! 実際のカメラの位置
  Vector3 m_actualPos;
  //! 実際のカメラの速度
  Vector3 m_velocity;
  //! 水平の追従する距離
  float m_horzDist;
  //! 垂直の追従する距離
  float m_vertDist;
  //! ターゲットまでの距離
  float m_targetDist;
  //! ばねの定数 (高いほど固くなる)
  float m_springConstant;

  // ターゲットからのオフセット
  Vector3 m_offset;
  // カメラの上方ベクトル
  Vector3 m_up;
  // ピッチの角速度
  float m_pitchSpeed;
  // ヨーの角速度
  float m_yawSpeed;
};
