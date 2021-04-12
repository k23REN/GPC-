#include "MathVector2.h"
#include "MathIncludes.h"

namespace MadCanyon
{
	const Vector2 Vector2::Right{ 1.0f,0.0f };
	const Vector2 Vector2::Left{ -1.0f,0.0f };
	const Vector2 Vector2::Up{ 0.0f,1.0f };
	const Vector2 Vector2::Down{ 0.0f,-1.0f };
	const Vector2 Vector2::Zero{ 0.0f,0.0f };
	const Vector2 Vector2::One{ 1.0f,1.0f };

	float Vector2::Length() const
	{
		return Vec2Length(Vec2Set(*this)).m128_f32[0];
	}

	float Vector2::LengthSquared() const
	{
		return Vec2Dot(Vec2Set(*this)).m128_f32[0];
	}

	float Vector2::Dot(const Vector2& aOther)const
	{
		return Vec2Dot(Vec2Set(*this), Vec2Set(aOther)).m128_f32[0];
	}
	float Vector2::sDot(const Vector2& v1, const Vector2& v2)
	{
		return Vec2Dot(Vec2Set(v1), Vec2Set(v2)).m128_f32[0];
	}

	Vector2 Vector2::Cross(const Vector2& rightOther) const
	{
		return Vec2Cross(Vec2Set(*this), Vec2Set(rightOther));
	}

	Vector2 Vector2::sCross(const Vector2& v1, Vector2& v2)
	{
		return Vec2Cross(Vec2Set(v1), Vec2Set(v2));
	}

	Vector2& Vector2::Normalize()
	{
		const float Length = this->Length();
		if (Length == 0.0f) { return *this; }

		const float Rate = 1.0f / Length;
		x *= Rate;
		y *= Rate;

		return *this;
	}

	Vector2& Vector2::Clamp(const Vector2& minVec, const Vector2& maxVec)
	{
		*this = VecClamp(Vec2Set(*this), Vec2Set(minVec), Vec2Set(maxVec));
		return *this;
	}

	Vector2& Vector2::Lerp(const Vector2& startVec, const Vector2& endVec, float time)
	{
		*this = VecLerp(Vec2Set(startVec), Vec2Set(endVec), Vec2Set(time));
		return *this;
	}

	Vector2& Vector2::SmoothStep(const Vector2& startVec, const Vector2& endVec, float time)
	{
		time = (time > 1.0f) ? 1.0f : ((time < 0.0f) ? 0.0f : time);  // ’l‚ð0`1‚ÉƒNƒ‰ƒ“ƒv
		time = time * time * (3.f - 2.f * time);
		*this = VecLerp(Vec2Set(startVec), Vec2Set(endVec), Vec2Set(time));
		return *this;
	}

	Vector2& Vector2::CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float time)
	{
		*this = Vec2CatmullRom(Vec2Set(v1), Vec2Set(v2), Vec2Set(v3), Vec2Set(v4), time);
		return *this;
	}
	Vector2& Vector2::Hermite(const Vector2& posVec1, const Vector2& tangentVec1, const Vector2& posVec2, const Vector2& tangentVec2, float time)
	{
		*this = Vec2Hermite(posVec1, tangentVec1, posVec2, tangentVec2, time);
		return *this;
	}

	Vector2& Vector2::TransformCoord(const Matrix44& m)
	{
		*this = Vec2TransformCoord(Vec2Set(*this), m);
		return *this;
	}

	Vector2& Vector2::TransformNormal(const Matrix44& m)
	{
		*this = Vec2TransformNormal(Vec2Set(*this), m);
		return *this;
	}




}