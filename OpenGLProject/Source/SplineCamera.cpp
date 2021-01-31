#include "SplineCamera.h"

#include "Actor.h"

Vector3 Spline::Compute(size_t a_startIdx, float t) const {
  //! a_startIdx�����E�O
  if (a_startIdx >= m_controlPoints.size()) {
    return m_controlPoints.back();
  } else if (a_startIdx == 0) {
    return m_controlPoints[a_startIdx];
  } else if (a_startIdx + 2 >= m_controlPoints.size()) {
    return m_controlPoints[a_startIdx];
  }

  //! p0����p3�܂ł̐���_���擾
  Vector3 p0 = m_controlPoints[a_startIdx - 1];
  Vector3 p1 = m_controlPoints[a_startIdx];
  Vector3 p2 = m_controlPoints[a_startIdx + 1];
  Vector3 p3 = m_controlPoints[a_startIdx + 2];

  //! Catmull-Rom�̕������ɂ���Ĉʒu���v�Z
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
  // t�̍X�V
  if (!m_paused) {
    m_t += m_speed * deltaTime;
    //! �K�v�ȏꍇ���̐���_�ɐi��
    //! ���X�s�[�h����������1�t���[���ŉz���Ȃ��悤����
    if (m_t >= 1.0f) {
      //! �o�H���I�����Ă��Ȃ���
      if (m_index < m_path.GetNumPoints() - 3) {
        m_index++;
        m_t = m_t - 1.0f;
      } else {
        //! �o�H�����ǂ�I�������ߋt�Đ�������
        Restart();
        std::vector<Vector3> rControlPoints = m_path.m_controlPoints;
        std::reverse(begin(rControlPoints), end(rControlPoints));
        m_path.m_controlPoints = rControlPoints;
      }
    }
  } 

  //! �J�����̈ʒu���A���݂̃C���f�b�N�X��t���狁�߂�
  Vector3 cameraPos = m_path.Compute(m_index, m_t);
  //! �����_�͂킸���ɐ�̈ʒu
  Vector3 target = m_path.Compute(m_index, m_t + 0.01f);
  //! �X�v���C�����㉺�t�ɂ��Ȃ�
  const Vector3 up = Vector3::UnitZ;
  Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
  SetViewMatrix(view);
}

void SplineCamera::Restart() {
  m_index = 1;
  m_t = 0.0f;
  m_paused = false;
}
