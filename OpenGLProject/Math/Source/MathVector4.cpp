#include "MathVector4.h"
#include "MathIncludes.h"

namespace MadCanyon
{
	static const Vector4 Zero = { 0.0f,0.0f,0.0f,0.0f };
	static const Vector4 One = { 1.0f,1.0f,1.0f,1.0f };

	float Vector4::Length() const
	{
		return Vec3Length(Vec3Set(x, y, z)).m128_f32[0];
	}
	float Vector4::LengthSquared() const
	{
		return Vec3Dot(Vec3Set(x, y, z)).m128_f32[0];
	}
	float Vector4::Dot(const Vector4& V) const
	{
		return Vec4Dot(Vec4Set(*this), Vec4Set(V)).m128_f32[0];
	}
	float Vector4::sDot(const Vector4& v1, const Vector4& v2)
	{
		return Vec4Dot(Vec4Set(v1), Vec4Set(v2)).m128_f32[0];
	}
	Vector4 Vector4::Cross(const Vector4& v1, Vector4& v2) const
	{
		return Vec4Cross(Vec4Set(*this), Vec4Set(v1), Vec4Set(v2));
	}
	Vector4 Vector4::sCross(const Vector4& v1, Vector4& v2, const Vector4& v3)
	{
		return Vec4Cross(Vec4Set(v1), Vec4Set(v2), Vec4Set(v3));
	}
	Vector4& Vector4::Normalize()
	{
		//x,y,zそれぞれのドット積を求める.
		//*this * *this.
		MVector lengthSq = _mm_mul_ps(*this, *this);
		// x = lengthSq.vector4_f32[1], y = lengthSq.vector4_f32[2]
		MVector temp = _mm_shuffle_ps(lengthSq, lengthSq, _MM_SHUFFLE(2, 1, 2, 1));

		lengthSq = _mm_add_ss(lengthSq, temp);

		temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));

		lengthSq = _mm_add_ss(lengthSq, temp);

		lengthSq = _mm_shuffle_ps(lengthSq, lengthSq, _MM_SHUFFLE(0, 0, 0, 0));

		MVector result = _mm_sqrt_ps(lengthSq);
		//4つの単精度浮動小数点値を0クリアしたものをzeroMaskに代入.
		MVector zeroMask = _mm_setzero_ps();

		zeroMask = _mm_cmpneq_ps(zeroMask, result);

		lengthSq = _mm_cmpneq_ps(lengthSq, Simd::MV_Infinity);

		result = _mm_div_ps(*this, result);

		result = _mm_and_ps(result, zeroMask);

		MVector temp1 = _mm_andnot_ps(lengthSq, Simd::MV_QNaN);
		MVector temp2 = _mm_and_ps(result, lengthSq);

		result = _mm_or_ps(temp1, temp2);
		*this = result;
		return *this;
	}
	Vector4& Vector4::Lerp(const Vector4& startVec, const Vector4& endVec, float time)
	{
		*this = VecLerp(Vec4Set(startVec), Vec4Set(endVec), Vec4Set(time));
		return *this;
	}
	Vector4& Vector4::Clamp(const Vector4& minVec, const Vector4& maxVec)
	{
		*this = VecClamp(*this, Vec4Set(minVec), Vec4Set(maxVec));
		return *this;
	}
	Vector4& Vector4::SmoothStep(const Vector4& startVec, const Vector4& endVec, float time)
	{
		time = (time > 1.0f) ? 1.0f : ((time < 0.0f) ? 0.0f : time);  // 値を0～1にクランプ
		time = time * time * (3.f - 2.f * time);
		*this = VecLerp(Vec4Set(startVec), Vec4Set(endVec), Vec4Set(time));
		return *this;
	}
	Vector4& Vector4::CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t)
	{
		*this = Vec4CatmullRom(Vec4Set(v1), Vec4Set(v2), Vec4Set(v3), Vec4Set(v4), t);
		return *this;
	}
	Vector4& Vector4::Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t)
	{
		*this = Vec4Hermite(v1, t1, v2, t2, t);
		return *this;
	}
	Vector4& Vector4::Transform(const Matrix44& m)
	{
		*this = Vec4Transform(Vec4Set(*this), m);
		return *this;
	}
}