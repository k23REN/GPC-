#include "SplineCamera.h"

#include "Actor.h"

Vector3 Spline::Compute(size_t a_startIdx, float t) const {
  //! a_startIdxが境界外
  if (a_startIdx >= m_controlPoints.size()) {
    return m_controlPoints.back();
  } else if (a_startIdx == 0) {
    return m_controlPoints[a_startIdx];
  } else if (a_startIdx + 2 >= m_controlPoints.size()) {
    return m_controlPoints[a_startIdx];
  }

  //! p0からp3までの制御点を取得
  Vector3 p0 = m_controlPoints[a_startIdx - 1];
  Vector3 p1 = m_controlPoints[a_startIdx];
  Vector3 p2 = m_controlPoints[a_startIdx + 1];
  Vector3 p3 = m_controlPoints[a_startIdx + 2];

  //! Catmull-Romの方程式によって位置を計算
  Vector3 position =
      0.5f * ((2.0f * p1) + (-1.0f * p0 + p2) * t +
              (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t +
              (-1.0f * p0 + 3.0f * p1 - 3.0f * p2 + p3) * t * t * t);
  return position;
}

SplineCamera::SplineCamera(Actor* owner)
    : CameraComponent(owner),
      m_index(1),
      m_t(0.0f),
      m_speed(0.5f),
      m_paused(true) {}

void SplineCamera::Update(float deltaTime) {
  CameraComponent::Update(deltaTime);
  // tの更新
  if (!m_paused) {
    m_t += m_speed * deltaTime;
    //! 必要な場合次の制御点に進む
    //! ※スピードが速すぎて1フレームで越さないよう調節
    if (m_t >= 1.0f) {
      //! 経路が終了していないか
      if (m_index < m_path.GetNumPoints() - 3) {
        m_index++;
        m_t = m_t - 1.0f;
      } else {
        //! 経路をたどり終えたため逆再生させる
        Restart();
        std::vector<Vector3> rControlPoints = m_path.m_controlPoints;
        std::reverse(begin(rControlPoints), end(rControlPoints));
        m_path.m_controlPoints = rControlPoints;
      }
    }
  } 

  //! カメラの位置を、現在のインデックスとtから求める
  Vector3 cameraPos = m_path.Compute(m_index, m_t);
  //! 注視点はわずかに先の位置
  Vector3 target = m_path.Compute(m_index, m_t + 0.01f);
  //! スプラインを上下逆にしない
  const Vector3 up = Vector3::UnitZ;
  Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
  SetViewMatrix(view);
}

void SplineCamera::Restart() {
  m_index = 1;
  m_t = 0.0f;
  m_paused = false;
}
