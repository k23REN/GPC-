#include "OrbitCamera.h"
#include "Actor.h"

OrbitCamera::OrbitCamera(Actor* owner)
	:CameraComponent(owner)
	,m_offset(-400.0f, 0.0f, 0.0f)
	,m_up(Vector3::UnitZ)
	,m_pitchSpeed(0.0f)
	,m_yawSpeed(0.0f)
{
}

void OrbitCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	//! ���[���h��y���������郈�[�̃N�H�[�^�j�I�����쐬
	Quaternion yaw(Vector3::UnitZ, m_yawSpeed * deltaTime);
	//! �J�����̃I�t�Z�b�g�Ə���x�N�g�������[�ŕϊ�
	m_offset = Vector3::Transform(m_offset, yaw);
	m_up = Vector3::Transform(m_up, yaw);

	//! �J�����̑O���ƉE�����v�Z����
	//! owner.position - (owner.position + offset) = -offset
	Vector3 forward = -1.0f * m_offset;
	forward.Normalize();
	Vector3 right = Vector3::Cross(m_up, forward);
	right.Normalize();
	
	//! �J�����̉E�������Ƃ���s�b�`�̃N�H�[�^�j�I�����쐬
	Quaternion pitch(right, m_pitchSpeed * deltaTime);
	//! �J�����̃I�t�Z�b�g�Ə���x�N�g�����s�b�`�ŕϊ�
	m_offset = Vector3::Transform(m_offset, pitch);
	m_up = Vector3::Transform(m_up, pitch);

	//! �ϊ��s����v�Z
	Vector3 target = m_pOwner->GetPosition();
	Vector3 cameraPos = target + m_offset;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, m_up);
	SetViewMatrix(view);
}
