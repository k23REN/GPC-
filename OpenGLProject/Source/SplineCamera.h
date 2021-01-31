#pragma once
#include "CameraComponent.h"
#include <vector>

struct Spline
{
	//! スプラインのための制御点
	//! 曲線に属する点がn個の場合n+2この点が必要
	std::vector<Vector3> m_controlPoints;
	//! a_startIdx = P1の区間でtの値に基づいて位置を計算
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
	//! カメラが追従するスプライン経路
	Spline m_path;
	
	//! 現在の制御点のインデックスとtの値
	size_t m_index;
	float m_t;
	//! 速度（tの増分/秒）
	float m_speed;
	//! 経路に沿って移動させるか
	bool m_paused;
};
