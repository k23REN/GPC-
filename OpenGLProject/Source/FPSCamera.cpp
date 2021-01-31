#include "FPSCamera.h"
#include "Actor.h"

FPSCamera::FPSCamera(Actor* owner)
	:CameraComponent(owner)
	,mPitchSpeed(0.0f)
	,mMaxPitch(Math::Pi / 3.0f)
	,mPitch(0.0f)
{
}

void FPSCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	//! �J�����̈ʒu�̓A�N�^�[�̈ʒu
	Vector3 cameraPos = m_pOwner->GetPosition();

	//! �s�b�`�̍X�V
	mPitch += mPitchSpeed * deltaTime;
	//! �s�b�`���}max�͈͓̔��ɂ���
	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);
	//! �s�b�`��]��\���N�H�[�^�j�I�������
	//! �I�[�i�[�̉E�����x�N�g�������Ƃ����]
	Quaternion q(m_pOwner->GetRight(), mPitch);
	
	//! �s�b�`�̃N�H�[�^�j�I�����g�p���đO���x�N�g������]
	Vector3 viewForward = Vector3::Transform(
		m_pOwner->GetForward(), q);
	//! �^�[�Q�b�g�ʒu��O��100�P�ʂɐݒ�
	Vector3 target = cameraPos + viewForward * 100.0f;
	//! ����x�N�g�����s�b�`�̃N�H�[�^�j�I���ŉ�]
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);
	
	//! �����s����쐬���A�r���[�ɐݒ�
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}
