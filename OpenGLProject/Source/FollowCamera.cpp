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
  //! ばね定数から減衰を計算
  float dampening = 2.0f * Math::Sqrt(m_springConstant);
  //! 目的位置を計算
  Vector3 idealPos = ComputeCameraPos();
  //! 現在位置と目的一の差を計算
  Vector3 diff = m_actualPos - idealPos;
  //! ばねによる加速度を計算
  Vector3 acel = -m_springConstant * diff - dampening * m_velocity;

  //! 速度を計算
  m_velocity += acel * deltaTime;
  //! カメラの座標を更新
  m_actualPos += m_velocity * deltaTime;
  //! ターゲットはアクターから前方に離れた座標に設定
  Vector3 target = m_pOwner->GetPosition() + m_pOwner->GetForward() * m_targetDist;

  Vector3 cameraPos;

  //! ピッチとヨー
  {
    //! ワールドのyを軸をするヨーのクォータニオンを作成
    Quaternion yaw(Vector3::UnitZ, m_yawSpeed * deltaTime);
    //! カメラのオフセットと上方ベクトルをヨーで変換
    m_offset = Vector3::Transform(m_offset, yaw);
    m_up = Vector3::Transform(m_up, yaw);

    //! カメラの前方と右方を計算する
    //! owner.position - (owner.position + offset) = -offset
    Vector3 forward = -1.0f * m_offset;
    forward.Normalize();
    Vector3 right = Vector3::Cross(m_up, forward);
    right.Normalize();

    //! カメラの右方を軸とするピッチのクォータニオンを作成
    Quaternion pitch(right, m_pitchSpeed * deltaTime);
    //! カメラのオフセットと上方ベクトルをピッチで変換
    m_offset = Vector3::Transform(m_offset, pitch);
    m_up = Vector3::Transform(m_up, pitch);
    cameraPos = m_actualPos + m_offset;
  }

  // Matrix4 view = Matrix4::CreateLookAt(cameraPos, m_actualPos, m_up);
  Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, m_up);
  SetViewMatrix(view);
}

void FollowCamera::SnapToIdeal() {
  //! 実際の位置は理想の位置と同じにする
  m_actualPos = ComputeCameraPos();
  // 速度をゼロに初期化
  m_velocity = Vector3::Zero;
  // 注視点とビューを計算
  Vector3 target =
      m_pOwner->GetPosition() + m_pOwner->GetForward() * m_targetDist;

  Matrix4 view = Matrix4::CreateLookAt(m_actualPos, target, Vector3::UnitZ);
  SetViewMatrix(view);
}

Vector3 FollowCamera::ComputeCameraPos() const {
  //! カメラの位置をアクターの上後方にセット
  Vector3 cameraPos = m_pOwner->GetPosition();
  cameraPos += Vector3::UnitZ * m_vertDist;
  return cameraPos;
}
