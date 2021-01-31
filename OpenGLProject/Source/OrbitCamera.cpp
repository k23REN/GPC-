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

	//! 変換行列を計算
	Vector3 target = m_pOwner->GetPosition();
	Vector3 cameraPos = target + m_offset;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, m_up);
	SetViewMatrix(view);
}
