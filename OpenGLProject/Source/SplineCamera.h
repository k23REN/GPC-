#pragma once
#include "CameraComponent.h"
#include <vector>

struct Spline
{
	//! �X�v���C���̂��߂̐���_
	//! �Ȑ��ɑ�����_��n�̏ꍇn+2���̓_���K�v
	std::vector<Vector3> m_controlPoints;
	//! a_startIdx = P1�̋�Ԃ�t�̒l�Ɋ�Â��Ĉʒu���v�Z
	Vector3 Compute(size_t a_startIdx, float t) const;
	size_t GetNumPoints() const { return m_controlPoints.size(); }
};

class SplineCamera : public CameraComponent
{
public:
	SplineCamera(class Actor* a_owner);

	void Update(float a_deltaTime) override;
	void Restart();

	void SetSpeed(float a_speed) { m_speed = a_speed; }
	void SetSpline(const Spline& a_spline) { m_path = a_spline; }
	void SetPaused(bool a_pause) { m_paused = a_pause; }
private:
	//! �J�������Ǐ]����X�v���C���o�H
	Spline m_path;
	
	//! ���݂̐���_�̃C���f�b�N�X��t�̒l
	size_t m_index;
	float m_t;
	//! ���x�it�̑���/�b�j
	float m_speed;
	//! �o�H�ɉ����Ĉړ������邩
	bool m_paused;
};
