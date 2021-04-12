#include "MathVector3.h"
#include "MathIncludes.h"

namespace MadCanyon
{
	const Vector3 Vector3::Right{ 1.0f,0.0f,0.0f };
	const Vector3 Vector3::Left{ -1.0f,0.0f,0.0f };
	const Vector3 Vector3::Up{ 0.0f,1.0f,0.0f };
	const Vector3 Vector3::Down{ 0.0f,-1.0f,0.0f };
	const Vector3 Vector3::Forward{ 0.0f,0.0f,1.0f };
	const Vector3 Vector3::Back{ 0.0f,0.0f,-1.0f };
	const Vector3 Vector3::Zero{ 0.0f,0.0f,0.0f };
	const Vector3 Vector3::One{ 1.0f,1.0f,1.0f };
	const Vector3 Vector3::Infinity{ 0x7F800000, 0x7F800000, 0x7F800000 };
	const Vector3 Vector3::NegativeInfinity{ 0xFF800000, 0xFF800000, 0xFF800000, };
	const Vector3 Vector3::PositiveInfinity{ 0xFF800000, 0xFF800000, 0xFF800000, };

	Vector3& Vector3::LookTo(const Vector3& dir, float angle)
	{
		// Šp“x
		float dot = Dot(dir);
		dot = std::clamp(dot, -1.0f, 1.0f);
		float ang = acos(dot);
		// Šp“x§ŒÀ
		if (ang > angle)ang = angle;
		// ‰ñ“]Ž²
		Vector3 vCrs = Vector3::sCross(*this, dir);
		vCrs.Normalize();
		if (vCrs.Length() == 0)vCrs.y = 1;
		// ‰ñ“]
		Quaternion qRota;
		qRota.CreateRotationAxis(vCrs, ang);
		Transform(qRota);

		return *this;
	}
	Vector4 Vector3::ToVec4(float w)
	{
		return Vector4(x, y, z, w);
	}

	float Vector3::Length() const
	{
		return Vec3Length(Vec3Set(*this)).m128_f32[0];
	}
	float Vector3::LengthSquared() const
	{
		return Vec3Dot(Vec3Set(*this)).m128_f32[0];
	}
	float Vector3::Dot(const Vector3& vec) const
	{
		return Vec3Dot(Vec3Set(*this), Vec3Set(vec)).m128_f32[0];
	}
	float Vector3::sDot(const Vector3& v1, const Vector3& v2)
	{
		return Vec3Dot(Vec3Set(v1), Vec3Set(v2)).m128_f32[0];
	}
	Vector3 Vector3::Cross(const Vector3& vec) const
	{
		return Vec3Cross(Vec3Set(*this), Vec3Set(vec));
	}
	Vector3 Vector3::sCross(const Vector3& vec1, const Vector3& vec2)
	{
		return Vec3Cross(Vec3Set(vec1), Vec3Set(vec2));
	}
	float Vector3::Angle(const Vector3& other) const
	{
		float dot = this->Normalized().Dot(other.Normalized());
		//Noramlize‚Ì¸“x‚ªŒ´ˆö‚Å0.0f`1.0fˆÈŠO‚Ì’l‚É‚È‚é‰Â”\«‚ª‚ ‚é‚½‚ß
		dot = Math::Clamp(dot, 0.0f, 1.0f);

		float angleRad = std::acosf(dot);
		if (!IsRight(other)) { angleRad *= -1.0f; }

		return angleRad;
	}
	Vector3& Vector3::Normalize()
	{
		*this = Vec3Normalize(Vec3Set(*this));
		return *this;
	}
	Vector3& Vector3::Clamp(const Vector3& minVec, const Vector3& maxVec)
	{
		*this = VecClamp(Vec3Set(*this), Vec3Set(minVec), Vec3Set(maxVec));
		return *this;
	}
	Vector3& Vector3::Lerp(const Vector3& startVec, const Vector3& endVec, float time)
	{
		*this = VecLerp(Vec3Set(startVec), Vec3Set(endVec), Vec3Set(time));
		return *this;
	}
	Vector3& Vector3::SmoothStep(const Vector3& startVec, const Vector3& endVec, float time)
	{
		time = (time > 1.0f) ? 1.0f : ((time < 0.0f) ? 0.0f : time);  // ’l‚ð0`1‚ÉƒNƒ‰ƒ“ƒv
		time = time * time * (3.f - 2.f * time);
		*this = VecLerp(Vec3Set(startVec), Vec3Set(endVec), Vec3Set(time));
		return *this;
	}
	Vector3& Vector3::CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t)
	{
		*this = Vec3CatmullRom(v1, v2, v3, v4, t);
		return *this;
	}
	Vector3& Vector3::Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t)
	{
		*this = Vec3Hermite(v1, t1, v2, t2, t);
		return *this;
	}
	Vector3& Vector3::Transform(const Quaternion& quat)
	{
		*this = Vec3Rotate(Vec3Set(*this), QuatSet(quat));
		return *this;
	}
	Vector3& Vector3::TransformCoord(const Matrix44& matrix)
	{
		*this = Vec3TransformCoord(Vec3Set(*this), matrix);
		return *this;
	}
	Vector3& Vector3::TransformNormal(const Matrix44& matrix)
	{
		*this = Vec3TransformNormal(Vec3Set(*this), matrix);
		return *this;
	}
}