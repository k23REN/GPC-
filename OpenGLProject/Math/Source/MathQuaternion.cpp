#include "MathQuaternion.h"
#include "MathVector2.h"
#include "MathVector3.h"
#include "MathMatrix44.h"

namespace MadCanyon
{
	const Quaternion Quaternion::Identity{ 0.0f,0.0f,0.0f,1.0f };
	inline Quaternion AddQuat(const Quaternion& left, const Quaternion& right)
	{
		return QuatAdd(QuatSet(left), QuatSet(right));
	}
	inline Quaternion SubQuat(const Quaternion& left, const Quaternion& right)
	{
		return QuatSub(QuatSet(left), QuatSet(right));
	}
	inline Quaternion MulQuat(const Quaternion& left, float value)
	{
		return QuatVecMul(QuatSet(left), QuatSet(value));
	}
	Quaternion& Quaternion::SetIdentity()
	{
		return *this = Quaternion::Identity;
	}

	Quaternion& Quaternion::CreateAngleAxis(float angleRadian, const Vector3& aAxisNorm)
	{
		this->Set(aAxisNorm * std::sinf(angleRadian * 0.5f), std::cosf(angleRadian * 0.5f));
		return *this;
	}

	Quaternion& Quaternion::CreateRotationAxis(const Vector3& axis, float angle)
	{
		if (axis.IsAllZero()) { return *this; }

		*this = QuatRotationAxis(Vec3Set(axis.Normalized()), angle);

		return *this;
	}

	Quaternion& Quaternion::CreateRotationFromAngles(float angleX, float angleY, float angleZ)
	{
		*this = QuatRotationRollPitchYaw(angleX, angleY, angleZ);
		return *this;
	}

	Quaternion& Quaternion::CreateRotationFromAngles(const Vector3& angle)
	{
		*this = QuatRotationRollPitchYaw(angle.x, angle.y, angle.z);
		return *this;
	}

	Quaternion& Quaternion::Conjugate()
	{
		*this = QuatConjugate(*this);
		return *this;
	}

	Quaternion& Quaternion::Inverse()
	{
		*this = QuaternionInverse(QuatSet(*this));
		return *this;
	}

	float Quaternion::Length() const
	{
		return QuatLength(QuatSet(*this), QuatSet(*this)).m128_f32[0];
	}
	Quaternion Quaternion::Multiply(const Quaternion& leftQuat, const Quaternion& rightQuat)
	{
		return QuatMul(leftQuat, rightQuat);
	}
	Vector3 Quaternion::MultiplyPoint(const Vector3& vec) const
	{
		return { (this->Inversed() * Quaternion { vec ,0.0f } * *this) };
	}
	// Quaternion同士の合成
	Quaternion operator* (const Quaternion& Q1, const Quaternion& Q2)
	{
		return QuatMul(Q1, Q2);
	}

	// Q1 * Q2の逆クォータニオン の合成
	Quaternion operator/ (const Quaternion& Q1, const Quaternion& Q2)
	{
		return QuatMul(Q1, QuaternionInverse(Q2));
	}
	Quaternion& Quaternion::CreateFromMatrix(const Matrix44& m)
	{
		// 行列の拡大成分を消す
		Matrix44 mTmp = m;
		mTmp.NormalizeScale();
		mTmp.Translation({ 0, 0, 0 });
		*this = QuaternionRotationMatrix(mTmp);
		return *this;
	}

	Quaternion& Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t)
	{
		*this = QuatSlerp(QuatSet(q1), QuatSet(q2), Vec4Set(t));
		return *this;
	}

	Quaternion& Quaternion::LookTo(const Vector3& dir, const Vector3& up)
	{
		Matrix44 m;
		//m.LookTo(dir, up);
		m = MatrixLookToLH(QuatSet(*this), Vec3Set(dir), Vec3Set(up));
		//CreateFromMatrix(m);
		return *this;
	}

	float Quaternion::Angle(const Quaternion& other) const
	{
		return std::acosf(this->Dot(other));
	}

	Quaternion& Quaternion::CreateLookRotation(const Vector3& lookNorm, const Vector3& worldUp)
	{
		const auto XAxis = worldUp.Cross(lookNorm);
		const auto YAxis = lookNorm.Cross(XAxis);
		const auto ZAxis = XAxis.Cross(YAxis);
		const auto RotationAxis = ZAxis.Cross(Vector3::Forward);
		return this->CreateAngleAxis(Vector3::Forward.Angle(RotationAxis), RotationAxis);
	}
}
