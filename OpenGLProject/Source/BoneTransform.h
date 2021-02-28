#pragma once
#include "Math.h"

class BoneTransform
{
public:
	Quaternion m_rotation;
	Vector3 m_translation;

	//s—ñ‚É•ÏŠ·
	Matrix4 ToMatrix() const;

	static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};
