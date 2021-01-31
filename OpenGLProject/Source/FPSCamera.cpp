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
	//! カメラの位置はアクターの位置
	Vector3 cameraPos = m_pOwner->GetPosition();

	//! ピッチの更新
	mPitch += mPitchSpeed * deltaTime;
	//! ピッチを±maxの範囲内にする
	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);
	//! ピッチ回転を表すクォータニオンを作る
	//! オーナーの右向きベクトルを軸とする回転
	Quaternion q(m_pOwner->GetRight(), mPitch);
	
	//! ピッチのクォータニオンを使用して前方ベクトルを回転
	Vector3 viewForward = Vector3::Transform(
		m_pOwner->GetForward(), q);
	//! ターゲット位置を前方100単位に設定
	Vector3 target = cameraPos + viewForward * 100.0f;
	//! 上方ベクトルもピッチのクォータニオンで回転
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);
	
	//! 注視行列を作成し、ビューに設定
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}
