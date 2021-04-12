#pragma once

namespace MadCanyon
{

	__declspec(align(16)) struct MVECTORU32
	{
		union
		{
			uint32_t u[4];
			__m128 v;
		};

		inline operator MVector() const { return v; }
#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
		inline operator __m128i() const { return _mm_castps_si128(v); }
		inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
	};

	__declspec(align(16)) struct MVECTORI32
	{
		union
		{
			int32_t i[4];
			__m128 v;
		};

		inline operator MVector() const { return v; }
#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
		inline operator __m128i() const { return _mm_castps_si128(v); }
		inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
	};

	__declspec(align(16)) struct MVECTORF32
	{
		union
		{
			float f[4];
			__m128 v;
		};

		inline operator MVector() const { return v; }
		inline operator const float* () const { return f; }
#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
		inline operator __m128i() const { return _mm_castps_si128(v); }
		inline operator __m128d() const { return _mm_castps_pd(v); }
#endif
	};
	static constexpr int Align = alignof(__m128);
	const __declspec(align(Align)) MVECTORU32 g_MCMask3 = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 } } };
	const __declspec(align(Align)) MVECTORI32 g_MCInfinity = { { { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 } } };
	const __declspec(align(Align)) MVECTORI32 g_MCQNaN = { { { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 } } };
	const __declspec(align(Align)) MVECTORF32 g_MCZero = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
	const __declspec(align(Align)) MVECTORF32 g_MCOne = { { { 1.0f, 1.0f, 1.0f, 1.0f } } };
	const __declspec(align(Align)) MVECTORF32 g_MCNegativeOne = { { { -1.0f, -1.0f, -1.0f, -1.0f } } };
	const __declspec(align(Align)) MVECTORU32 g_MCNegativeZero = { { { 0x80000000, 0x80000000, 0x80000000, 0x80000000 } } };
	const __declspec(align(Align)) MVECTORI32 g_MCAbsMask = { { { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF } } };

	namespace Simd
	{
		constexpr float MC_PI = 3.141592654f;
		constexpr float MC_2PI = 6.283185307f;
		constexpr float MC_1DIVPI = 0.318309886f;
		constexpr float MC_1DIV2PI = 0.159154943f;
		constexpr float MC_PIDIV2 = 1.570796327f;
		constexpr float MC_PIDIV4 = 0.785398163f;

		constexpr uint32_t MC_SELECT_0 = 0x00000000;
		constexpr uint32_t MC_SELECT_1 = 0xFFFFFFFF;

		constexpr uint32_t MC_PERMUTE_0X = 0;
		constexpr uint32_t MC_PERMUTE_0Y = 1;
		constexpr uint32_t MC_PERMUTE_0Z = 2;
		constexpr uint32_t MC_PERMUTE_0W = 3;
		constexpr uint32_t MC_PERMUTE_1X = 4;
		constexpr uint32_t MC_PERMUTE_1Y = 5;
		constexpr uint32_t MC_PERMUTE_1Z = 6;
		constexpr uint32_t MC_PERMUTE_1W = 7;

		constexpr uint32_t MC_SWIZZLE_X = 0;
		constexpr uint32_t MC_SWIZZLE_Y = 1;
		constexpr uint32_t MC_SWIZZLE_Z = 2;
		constexpr uint32_t MC_SWIZZLE_W = 3;

		extern const _declspec(selectany) MVector MV_PI = { MC_PI, MC_PI, MC_PI, MC_PI, };
		extern const _declspec(selectany) MVector MV_2PI = { MC_2PI, MC_2PI, MC_2PI, MC_2PI, };
		extern const _declspec(selectany) MVector MV_Infinity = { Math::Infinity<float>,Math::Infinity<float>,Math::Infinity<float>,Math::Infinity<float> };
		extern const _declspec(selectany) MVector MV_QNaN = { Math::QuietNaN<float>,Math::QuietNaN<float>,Math::QuietNaN<float>,Math::QuietNaN<float> };
		extern const _declspec(selectany) MVECTORF32 MV_IdentityR0 = { { { 1.0f, 0.0f, 0.0f, 0.0f } } };
		extern const _declspec(selectany) MVECTORF32 MV_IdentityR1 = { { { 0.0f, 1.0f, 0.0f, 0.0f } } };
		extern const _declspec(selectany) MVECTORF32 MV_IdentityR2 = { { { 0.0f, 0.0f, 1.0f, 0.0f } } };
		extern const _declspec(selectany) MVECTORF32 MV_IdentityR3 = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
		extern const _declspec(selectany) MVECTORF32 MV_NegateX = { { { -1.0f, 1.0f, 1.0f, 1.0f } } };
		extern const _declspec(selectany) MVECTORF32 MV_NegateY = { { { 1.0f, -1.0f, 1.0f, 1.0f } } };
		extern const _declspec(selectany) MVECTORF32 MV_NegateZ = { { { 1.0f, 1.0f, -1.0f, 1.0f } } };
		extern const _declspec(selectany) MVECTORF32 MV_NegateW = { { { 1.0f, 1.0f, 1.0f, -1.0f } } };
		extern const _declspec(selectany) MVECTORU32 MV_MaskXY = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000 } } };
		extern const _declspec(selectany) MVECTORU32 MV_Mask3 = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 } } };
		extern const _declspec(selectany) MVECTORU32 MV_MaskX = { { { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 } } };
		extern const _declspec(selectany) MVECTORU32 MV_MaskY = { { { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 } } };
		extern const _declspec(selectany) MVECTORU32 MV_MaskZ = { { { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 } } };
		extern const _declspec(selectany) MVECTORU32 MV_MaskW = { { { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF } } };
		extern const _declspec(selectany) MVECTORU32 MV_Select1000 = { { { MC_SELECT_1, MC_SELECT_0, MC_SELECT_0, MC_SELECT_0 } } };
		extern const _declspec(selectany) MVECTORU32 MV_Select1100 = { { { MC_SELECT_1, MC_SELECT_1, MC_SELECT_0, MC_SELECT_0 } } };
		extern const _declspec(selectany) MVECTORU32 MV_Select1110 = { { { MC_SELECT_1, MC_SELECT_1, MC_SELECT_1, MC_SELECT_0 } } };
		extern const _declspec(selectany) MVECTORU32 MV_Select1011 = { { { MC_SELECT_1, MC_SELECT_0, MC_SELECT_1, MC_SELECT_1 } } };
		extern const _declspec(selectany) MVECTORF32 MV_One = { { { 1.0f, 1.0f, 1.0f, 1.0f } } };
		extern const _declspec(selectany) MVECTORF32 MV_One3 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };
		extern const _declspec(selectany) MVECTORF32 MV_OneHalf = { { { 0.5f, 0.5f, 0.5f, 0.5f } } };
		extern const _declspec(selectany) MVECTORF32 MV_Zero = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
		extern const _declspec(selectany) MVECTORF32 MV_NegativeOne = { { { -1.0f, -1.0f, -1.0f, -1.0f } } };
		extern const _declspec(selectany) MVECTORF32 MV_Epsilon = { { { 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f } } };
		extern const _declspec(selectany) MVECTORI32 MV_AbsMask = { { { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF } } };

	}



	__declspec(align(16)) struct MMATRIX
	{
#ifdef _XM_NO_INTRINSICS_
		union
		{
			MVector r[4];
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};
#else
		MVector r[4];
#endif

		MMATRIX() = default;

		//MMATRIX(const DirectX::XMMATRIX&) = default;
		MMATRIX(const DirectX::XMMATRIX& M);
		MMATRIX(const MMATRIX& M);

#if defined(_MSC_VER) && (_MSC_FULL_VER < 191426431)
		MMATRIX& operator= (const MMATRIX& M) noexcept { r[0] = M.r[0]; r[1] = M.r[1]; r[2] = M.r[2]; r[3] = M.r[3]; return *this; }
#else
		MMATRIX& operator=(const MMATRIX&) = default;

		MMATRIX(MMATRIX&&) = default;
		MMATRIX& operator=(MMATRIX&&) = default;
#endif

		constexpr MMATRIX(MVector R0, MVector R1, MVector R2, MVector R3) : r{ R0,R1,R2,R3 } {}
		MMATRIX(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);
		explicit MMATRIX(_In_reads_(16) const float* pArray);

#ifdef _XM_NO_INTRINSICS_
		float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
		float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
#endif

		//MMATRIX    operator+ () const { return *this; }
		MMATRIX    operator- () const;

		MMATRIX& MC_VECCALL     operator+= (MMATRIX& M);
		MMATRIX& MC_VECCALL     operator-= (MMATRIX& M);
		MMATRIX& MC_VECCALL     operator*= (MMATRIX& M);
		MMATRIX& operator*= (float S);
		MMATRIX& operator/= (float S);

		MMATRIX    MC_VECCALL     operator+ (MMATRIX& M) const;
		MMATRIX    MC_VECCALL     operator- (MMATRIX& M) const;
		MMATRIX    MC_VECCALL     operator* (MMATRIX M) const;
		MMATRIX    operator* (float S) const;
		MMATRIX    operator/ (float S) const;

		friend MMATRIX     MC_VECCALL     operator* (float S, MMATRIX M);
	};

	class Vector2;
	class Vector3;
	class Vector4;
	class Quaternion;
	class Matrix44;

	MVector MC_VECCALL  Vec2Set(const Vector2& v);
	MVector MC_VECCALL  Vec2Set(float x, float y);
	MVector MC_VECCALL  Vec2Set(float value);
	void MC_VECCALL  MVecStore(Vector2* out, const MVector& in);
	MVector MC_VECCALL  MLoadVec2(const Vector2* in);
	MVector MC_VECCALL  Vec2Add(MVector left, MVector right);
	MVector MC_VECCALL  Vec2Sub(MVector left, MVector right);
	MVector MC_VECCALL  Vec2Mul(MVector left, MVector right);
	MVector MC_VECCALL  Vec2Div(MVector left, MVector right);
	MVector MC_VECCALL Vec2Length(MVector v);
	MVector MC_VECCALL Vec2Dot(MVector v);
	MVector MC_VECCALL Vec2Cross(MVector left, MVector right);
	MVector MC_VECCALL VecClamp(MVector own, MVector min, MVector max);
	MVector MC_VECCALL VecLerp(MVector start, MVector end, MVector time);
	MVector MC_VECCALL Vec2TransformCoord(MVector v, MMATRIX m);
	MVector MC_VECCALL Vec2TransformNormal(MVector v, MMATRIX m);
	MVector MC_VECCALL Vec2CatmullRom(MVector v1, MVector v2, MVector v3, MVector v4, float time);
	MVector MC_VECCALL Vec2Hermite(MVector posVec1, MVector tangentVec1, MVector posVec2, MVector tangentVec2, float time);


	MVector MC_VECCALL Vec3Set(const Vector3& v);
	MVector MC_VECCALL Vec3Set(float x, float y, float z);
	MVector MC_VECCALL  Vec3Set(float value);
	void MC_VECCALL  MVecStore(Vector3* out, const MVector& in);
	MVector MC_VECCALL  MLoadVec3(const Vector3* in);
	MVector MC_VECCALL  Vec3Add(MVector left, MVector right);
	MVector MC_VECCALL  Vec3Sub(MVector left, MVector right);
	MVector MC_VECCALL  Vec3Mul(MVector left, MVector right);
	MVector MC_VECCALL  Vec3Div(MVector left, MVector right);
	MVector MC_VECCALL Vec3Length(MVector v);
	MVector MC_VECCALL Vec3Dot(MVector v);
	MVector MC_VECCALL Vec3Dot(MVector left, MVector right);
	MVector MC_VECCALL Vec3Cross(MVector left, MVector right);
	MVector MC_VECCALL Vec3Normalize(MVector v);
	MVector MC_VECCALL Vec3Rotate(MVector v, MVector quat);
	MVector MC_VECCALL Vec3TransformCoord(MVector v, MMATRIX m);
	MVector MC_VECCALL Vec3TransformNormal(MVector v, MMATRIX m);
	MVector MC_VECCALL Vec3CatmullRom(MVector v1, MVector v2, MVector v3, MVector v4, float time);
	MVector MC_VECCALL Vec3Hermite(MVector posVec1, MVector tangentVec1, MVector posVec2, MVector tangentVec2, float time);

	MVector MC_VECCALL Vec4Set(const Vector4& v);
	MVector MC_VECCALL Vec4Set(float x, float y, float z, float w);
	MVector MC_VECCALL  Vec4Set(float value);
	void MC_VECCALL  MVecStore(Vector4* out, const MVector& in);
	MVector MC_VECCALL MLoadVec4(const Vector4* in);
	MVector MC_VECCALL  Vec4Add(MVector left, MVector right);
	MVector MC_VECCALL  Vec4Sub(MVector left, MVector right);
	MVector MC_VECCALL  Vec4Mul(MVector left, MVector right);
	MVector MC_VECCALL  Vec4Div(MVector left, MVector right);
	MVector MC_VECCALL Vec4Length(MVector v);
	MVector MC_VECCALL Vec4Dot(MVector left, MVector right);
	MVector MC_VECCALL Vec4Cross(MVector axisNorm, MVector left, MVector right);
	MVector MC_VECCALL Vec4Normalize(MVector V);
	MVector MC_VECCALL Vec4Transform(MVector V, MMATRIX M);
	MVector MC_VECCALL Vec4CatmullRom(MVector v1, MVector v2, MVector v3, MVector v4, float time);
	MVector MC_VECCALL Vec4Hermite(MVector posVec1, MVector tangentVec1, MVector posVec2, MVector tangentVec2, float time);

	MVector MC_VECCALL  QuatSet(const Quaternion& v);
	MVector MC_VECCALL  QuatSet(float x, float y, float z, float w);
	MVector MC_VECCALL  QuatSet(float value);
	void MC_VECCALL  MQuatStore(Quaternion* out, const MVector& in);
	MVector MC_VECCALL  MLoadQuat(const Quaternion* in);
	MVector MC_VECCALL  QuatAdd(MVector left, MVector right);
	MVector MC_VECCALL  QuatSub(MVector left, MVector right);
	MVector MC_VECCALL  QuatVecMul(MVector left, MVector right);
	MVector MC_VECCALL  QuatMul(MVector left, MVector right);
	MVector MC_VECCALL  QuatDiv(MVector left, MVector right);
	MVector MC_VECCALL  QuatLength(MVector left, MVector right);
	MVector MC_VECCALL QuatSlerp(MVector left, MVector right, MVector time);
	MVector MC_VECCALL  QuatRotationNormal(MVector normalAxis, float angle);
	MVector MC_VECCALL  QuatRotationAxis(MVector normalAxis, float angle);
	MVector MC_VECCALL QuatRotationRollPitchYawFromVector(
		MVector Angles // <Pitch, Yaw, Roll, 0>
	);
	MVector MC_VECCALL QuatRotationRollPitchYaw(float Pitch, float Yaw, float Roll);
	MVector MC_VECCALL QuatConjugate(MVector v);
	MVector MC_VECCALL QuaternionRotationMatrix(MMATRIX M);
	MVector MC_VECCALL QuaternionInverse(MVector Q);
	MVector MC_VECCALL QuaternionNormalize(MVector Q);

	MVector MC_VECCALL VectorNegate(MVector V);
	MVector MC_VECCALL VectorScale(MVector V, float scaleFactor);

	MMATRIX MC_VECCALL LoadMatrix(const Matrix44* src);
	MMATRIX MC_VECCALL MatrixLookToLH(MVector eyePos, MVector eyeDir, MVector upDir);
	
	inline MMATRIX::MMATRIX(const DirectX::XMMATRIX& M)
	{
		memcpy_s(this, sizeof(float) * 16, &M, sizeof(DirectX::XMMATRIX));
	}

	inline MMATRIX::MMATRIX(const MMATRIX& M)
	{
		*this = M;
	}

	inline MMATRIX::MMATRIX
	(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	)
	{
		r[0] = Vec4Set(m00, m01, m02, m03);
		r[1] = Vec4Set(m10, m11, m12, m13);
		r[2] = Vec4Set(m20, m21, m22, m23);
		r[3] = Vec4Set(m30, m31, m32, m33);
	}

	//------------------------------------------------------------------------------
	_Use_decl_annotations_
		inline MMATRIX::MMATRIX
		(
			const float* pArray
		)
	{
		assert(pArray != nullptr);
		r[0] = MLoadVec4(reinterpret_cast<const Vector4*>(pArray));
		r[1] = MLoadVec4(reinterpret_cast<const Vector4*>(pArray + 4));
		r[2] = MLoadVec4(reinterpret_cast<const Vector4*>(pArray + 8));
		r[3] = MLoadVec4(reinterpret_cast<const Vector4*>(pArray + 12));
	}

	//------------------------------------------------------------------------------

	inline MMATRIX MMATRIX::operator- () const
	{
		MMATRIX R;
		R.r[0] = VectorNegate(r[0]);
		R.r[1] = VectorNegate(r[1]);
		R.r[2] = VectorNegate(r[2]);
		R.r[3] = VectorNegate(r[3]);
		return R;
	}

	//------------------------------------------------------------------------------

	inline MMATRIX& MC_VECCALL MMATRIX::operator+= (MMATRIX& M)
	{
		r[0] = Vec4Add(r[0], M.r[0]);
		r[1] = Vec4Add(r[1], M.r[1]);
		r[2] = Vec4Add(r[2], M.r[2]);
		r[3] = Vec4Add(r[3], M.r[3]);
		return *this;
	}

	//------------------------------------------------------------------------------

	inline MMATRIX& MC_VECCALL MMATRIX::operator-= (MMATRIX& M)
	{
		r[0] = Vec4Sub(r[0], M.r[0]);
		r[1] = Vec4Sub(r[1], M.r[1]);
		r[2] = Vec4Sub(r[2], M.r[2]);
		r[3] = Vec4Sub(r[3], M.r[3]);
		return *this;
	}

	//------------------------------------------------------------------------------

	inline MMATRIX& MC_VECCALL MMATRIX::operator*=(MMATRIX& M)
	{
		MMATRIX mResult;
		// Use vW to hold the original row
		MVector vW = this->r[0];
		MVector vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		MVector vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		MVector vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		// Perform the operation on the first row
		vX = _mm_mul_ps(vX, M.r[0]);
		vY = _mm_mul_ps(vY, M.r[1]);
		vZ = _mm_mul_ps(vZ, M.r[2]);
		vW = _mm_mul_ps(vW, M.r[3]);
		// Perform a binary add to reduce cumulative errors
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.r[0] = vX;
		// Repeat for the other 3 rows
		vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		vW = this->r[1];
		vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, M.r[0]);
		vY = _mm_mul_ps(vY, M.r[1]);
		vZ = _mm_mul_ps(vZ, M.r[2]);
		vW = _mm_mul_ps(vW, M.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.r[1] = vX;
		vW = this->r[2];
		vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, M.r[0]);
		vY = _mm_mul_ps(vY, M.r[1]);
		vZ = _mm_mul_ps(vZ, M.r[2]);
		vW = _mm_mul_ps(vW, M.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.r[2] = vX;
		vW = this->r[3];
		vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, M.r[0]);
		vY = _mm_mul_ps(vY, M.r[1]);
		vZ = _mm_mul_ps(vZ, M.r[2]);
		vW = _mm_mul_ps(vW, M.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.r[3] = vX;
		*this = mResult;
		return *this;
	}

	//------------------------------------------------------------------------------

	inline MMATRIX& MMATRIX::operator*= (float S)
	{
		r[0] = VectorScale(r[0], S);
		r[1] = VectorScale(r[1], S);
		r[2] = VectorScale(r[2], S);
		r[3] = VectorScale(r[3], S);
		return *this;
	}

	//------------------------------------------------------------------------------

	inline MMATRIX& MMATRIX::operator/= (float S)
	{
#if defined(_XM_NO_INTRINSICS_)
		MVector vS = XMVectorReplicate(S);
		r[0] = XMVectorDivide(r[0], vS);
		r[1] = XMVectorDivide(r[1], vS);
		r[2] = XMVectorDivide(r[2], vS);
		r[3] = XMVectorDivide(r[3], vS);
		return *this;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
		float32x4_t vS = vdupq_n_f32(S);
		r[0] = vdivq_f32(r[0], vS);
		r[1] = vdivq_f32(r[1], vS);
		r[2] = vdivq_f32(r[2], vS);
		r[3] = vdivq_f32(r[3], vS);
#else
		// 2 iterations of Newton-Raphson refinement of reciprocal
		float32x2_t vS = vdup_n_f32(S);
		float32x2_t R0 = vrecpe_f32(vS);
		float32x2_t S0 = vrecps_f32(R0, vS);
		R0 = vmul_f32(S0, R0);
		S0 = vrecps_f32(R0, vS);
		R0 = vmul_f32(S0, R0);
		float32x4_t Reciprocal = vcombine_u32(R0, R0);
		r[0] = vmulq_f32(r[0], Reciprocal);
		r[1] = vmulq_f32(r[1], Reciprocal);
		r[2] = vmulq_f32(r[2], Reciprocal);
		r[3] = vmulq_f32(r[3], Reciprocal);
#endif
		return *this;
#elif defined(_XM_SSE_INTRINSICS_)
		__m128 vS = _mm_set_ps1(S);
		r[0] = _mm_div_ps(r[0], vS);
		r[1] = _mm_div_ps(r[1], vS);
		r[2] = _mm_div_ps(r[2], vS);
		r[3] = _mm_div_ps(r[3], vS);
		return *this;
#endif
	}

	//------------------------------------------------------------------------------

	inline MMATRIX MC_VECCALL MMATRIX::operator+ (MMATRIX& M) const
	{
		MMATRIX R;
		R.r[0] = Vec4Add(r[0], M.r[0]);
		R.r[1] = Vec4Add(r[1], M.r[1]);
		R.r[2] = Vec4Add(r[2], M.r[2]);
		R.r[3] = Vec4Add(r[3], M.r[3]);
		return R;
	}

	//------------------------------------------------------------------------------

	inline MMATRIX MC_VECCALL MMATRIX::operator- (MMATRIX& M) const
	{
		MMATRIX R;
		R.r[0] = Vec4Sub(r[0], M.r[0]);
		R.r[1] = Vec4Sub(r[1], M.r[1]);
		R.r[2] = Vec4Sub(r[2], M.r[2]);
		R.r[3] = Vec4Sub(r[3], M.r[3]);
		return R;
	}

	//------------------------------------------------------------------------------

	inline MMATRIX MC_VECCALL MMATRIX::operator*(MMATRIX M) const
	{
		MMATRIX mResult;
		// Use vW to hold the original row
		MVector vW = this->r[0];
		MVector vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		MVector vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		MVector vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		// Perform the operation on the first row
		vX = _mm_mul_ps(vX, M.r[0]);
		vY = _mm_mul_ps(vY, M.r[1]);
		vZ = _mm_mul_ps(vZ, M.r[2]);
		vW = _mm_mul_ps(vW, M.r[3]);
		// Perform a binary add to reduce cumulative errors
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.r[0] = vX;
		// Repeat for the other 3 rows
		vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		vW = this->r[1];
		vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, M.r[0]);
		vY = _mm_mul_ps(vY, M.r[1]);
		vZ = _mm_mul_ps(vZ, M.r[2]);
		vW = _mm_mul_ps(vW, M.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.r[1] = vX;
		vW = this->r[2];
		vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, M.r[0]);
		vY = _mm_mul_ps(vY, M.r[1]);
		vZ = _mm_mul_ps(vZ, M.r[2]);
		vW = _mm_mul_ps(vW, M.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.r[2] = vX;
		vW = this->r[3];
		vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
		vX = _mm_mul_ps(vX, M.r[0]);
		vY = _mm_mul_ps(vY, M.r[1]);
		vZ = _mm_mul_ps(vZ, M.r[2]);
		vW = _mm_mul_ps(vW, M.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.r[3] = vX;
		return mResult;

	}

	//------------------------------------------------------------------------------

	inline MMATRIX MMATRIX::operator* (float S) const
	{
		MMATRIX R;
		R.r[0] = VectorScale(r[0], S);
		R.r[1] = VectorScale(r[1], S);
		R.r[2] = VectorScale(r[2], S);
		R.r[3] = VectorScale(r[3], S);
		return R;
	}

	//------------------------------------------------------------------------------

	inline MMATRIX MMATRIX::operator/ (float S) const
	{
#if defined(_XM_NO_INTRINSICS_)
		MVector vS = XMVectorReplicate(S);
		MMATRIX R;
		R.r[0] = XMVectorDivide(r[0], vS);
		R.r[1] = XMVectorDivide(r[1], vS);
		R.r[2] = XMVectorDivide(r[2], vS);
		R.r[3] = XMVectorDivide(r[3], vS);
		return R;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
		float32x4_t vS = vdupq_n_f32(S);
		MMATRIX R;
		R.r[0] = vdivq_f32(r[0], vS);
		R.r[1] = vdivq_f32(r[1], vS);
		R.r[2] = vdivq_f32(r[2], vS);
		R.r[3] = vdivq_f32(r[3], vS);
#else
		// 2 iterations of Newton-Raphson refinement of reciprocal
		float32x2_t vS = vdup_n_f32(S);
		float32x2_t R0 = vrecpe_f32(vS);
		float32x2_t S0 = vrecps_f32(R0, vS);
		R0 = vmul_f32(S0, R0);
		S0 = vrecps_f32(R0, vS);
		R0 = vmul_f32(S0, R0);
		float32x4_t Reciprocal = vcombine_u32(R0, R0);
		MMATRIX R;
		R.r[0] = vmulq_f32(r[0], Reciprocal);
		R.r[1] = vmulq_f32(r[1], Reciprocal);
		R.r[2] = vmulq_f32(r[2], Reciprocal);
		R.r[3] = vmulq_f32(r[3], Reciprocal);
#endif
		return R;
#elif defined(_XM_SSE_INTRINSICS_)
		__m128 vS = _mm_set_ps1(S);
		MMATRIX R;
		R.r[0] = _mm_div_ps(r[0], vS);
		R.r[1] = _mm_div_ps(r[1], vS);
		R.r[2] = _mm_div_ps(r[2], vS);
		R.r[3] = _mm_div_ps(r[3], vS);
		return R;
#endif
	}

	//------------------------------------------------------------------------------

	inline MMATRIX MC_VECCALL operator*
		(
			float S,
			MMATRIX& M
			)
	{
		MMATRIX R;
		R.r[0] = VectorScale(M.r[0], S);
		R.r[1] = VectorScale(M.r[1], S);
		R.r[2] = VectorScale(M.r[2], S);
		R.r[3] = VectorScale(M.r[3], S);
		return R;
	}

	//-------------DirectXMath.hÇÃ1617çs------------------
	//https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorpermute
	// PermuteHelper internal template (SSE only)
	namespace Internal
	{
		// Slow path fallback for permutes that do not map to a single SSE shuffle opcode.
		template<uint32_t Shuffle, bool WhichX, bool WhichY, bool WhichZ, bool WhichW> struct PermuteHelper
		{
			static MVector     MC_VECCALL     Permute(MVector v1, MVector v2)
			{
				static const MVECTORU32 selectMask =
				{ { {
						WhichX ? 0xFFFFFFFF : 0,
						WhichY ? 0xFFFFFFFF : 0,
						WhichZ ? 0xFFFFFFFF : 0,
						WhichW ? 0xFFFFFFFF : 0,
				} } };

				MVector shuffled1 = XM_PERMUTE_PS(v1, Shuffle);
				MVector shuffled2 = XM_PERMUTE_PS(v2, Shuffle);

				MVector masked1 = _mm_andnot_ps(selectMask, shuffled1);
				MVector masked2 = _mm_and_ps(selectMask, shuffled2);

				return _mm_or_ps(masked1, masked2);
			}
		};

		// Fast path for permutes that only read from the first vector.
		template<uint32_t Shuffle> struct PermuteHelper<Shuffle, false, false, false, false>
		{
			static MVector     MC_VECCALL     Permute(MVector v1, MVector) { return XM_PERMUTE_PS(v1, Shuffle); }
		};

		// Fast path for permutes that only read from the second vector.
		template<uint32_t Shuffle> struct PermuteHelper<Shuffle, true, true, true, true>
		{
			static MVector     MC_VECCALL     Permute(MVector, MVector v2) { return XM_PERMUTE_PS(v2, Shuffle); }
		};

		// Fast path for permutes that read XY from the first vector, ZW from the second.
		template<uint32_t Shuffle> struct PermuteHelper<Shuffle, false, false, true, true>
		{
			static MVector     MC_VECCALL     Permute(MVector v1, MVector v2) { return _mm_shuffle_ps(v1, v2, Shuffle); }
		};

		// Fast path for permutes that read XY from the second vector, ZW from the first.
		template<uint32_t Shuffle> struct PermuteHelper<Shuffle, true, true, false, false>
		{
			static MVector     MC_VECCALL     Permute(MVector v1, MVector v2) { return _mm_shuffle_ps(v2, v1, Shuffle); }
		};
	}

	// General permute template
	template<uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW>
	inline MVector     MC_VECCALL     MVectorPermute(MVector V1, MVector V2)
	{
		static_assert(PermuteX <= 7, "PermuteX template parameter out of range");
		static_assert(PermuteY <= 7, "PermuteY template parameter out of range");
		static_assert(PermuteZ <= 7, "PermuteZ template parameter out of range");
		static_assert(PermuteW <= 7, "PermuteW template parameter out of range");

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
		const uint32_t Shuffle = _MM_SHUFFLE(PermuteW & 3, PermuteZ & 3, PermuteY & 3, PermuteX & 3);

		const bool WhichX = PermuteX > 3;
		const bool WhichY = PermuteY > 3;
		const bool WhichZ = PermuteZ > 3;
		const bool WhichW = PermuteW > 3;

		return Internal::PermuteHelper<Shuffle, WhichX, WhichY, WhichZ, WhichW>::Permute(V1, V2);
#else

		return MVectorPermute(V1, V2, PermuteX, PermuteY, PermuteZ, PermuteW);

#endif
	}


#define M3RANKDECOMPOSE(a, b, c, x, y, z)      \
    if((x) < (y))                   \
    {                               \
        if((y) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 1;                \
            (c) = 0;                \
        }                           \
        else                        \
        {                           \
            (a) = 1;                \
                                    \
            if((x) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 0;            \
            }                       \
            else                    \
            {                       \
                (b) = 0;            \
                (c) = 2;            \
            }                       \
        }                           \
    }                               \
    else                            \
    {                               \
        if((x) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 0;                \
            (c) = 1;                \
        }                           \
        else                        \
        {                           \
            (a) = 0;                \
                                    \
            if((y) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 1;            \
            }                       \
            else                    \
            {                       \
                (b) = 1;            \
                (c) = 2;            \
            }                       \
        }                           \
    }

#define M3_DECOMP_EPSILON 0.0001f

	//-------------DirectXMath.hÇÃ1766çs------------------

	// General swizzle template
	template<uint32_t SwizzleX, uint32_t SwizzleY, uint32_t SwizzleZ, uint32_t SwizzleW>
	inline MVector     MC_VECCALL     MVectorSwizzle(MVector V)
	{
		static_assert(SwizzleX <= 3, "SwizzleX template parameter out of range");
		static_assert(SwizzleY <= 3, "SwizzleY template parameter out of range");
		static_assert(SwizzleZ <= 3, "SwizzleZ template parameter out of range");
		static_assert(SwizzleW <= 3, "SwizzleW template parameter out of range");

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
		return XM_PERMUTE_PS(V, _MM_SHUFFLE(SwizzleW, SwizzleZ, SwizzleY, SwizzleX));
#else

		return MVectorSwizzle(V, SwizzleX, SwizzleY, SwizzleZ, SwizzleW);

#endif
	}

	// Specialized swizzles
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<0, 1, 2, 3>(MVector V) { return V; }

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<0, 1, 0, 1>(MVector V) { return _mm_movelh_ps(V, V); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<2, 3, 2, 3>(MVector V) { return _mm_movehl_ps(V, V); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<0, 0, 1, 1>(MVector V) { return _mm_unpacklo_ps(V, V); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<2, 2, 3, 3>(MVector V) { return _mm_unpackhi_ps(V, V); }
#endif

#if defined(_XM_SSE3_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<0, 0, 2, 2>(MVector V) { return _mm_moveldup_ps(V); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<1, 1, 3, 3>(MVector V) { return _mm_movehdup_ps(V); }
#endif

#if defined(_XM_AVX2_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<0, 0, 0, 0>(MVector V) { return _mm_broadcastss_ps(V); }
#endif

#if defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)

	template<> inline MVector      MC_VECCALL     MVectorSwizzle<0, 0, 0, 0>(MVector V) { return vdupq_lane_f32(vget_low_f32(V), 0); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<1, 1, 1, 1>(MVector V) { return vdupq_lane_f32(vget_low_f32(V), 1); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<2, 2, 2, 2>(MVector V) { return vdupq_lane_f32(vget_high_f32(V), 0); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<3, 3, 3, 3>(MVector V) { return vdupq_lane_f32(vget_high_f32(V), 1); }

	template<> inline MVector      MC_VECCALL     MVectorSwizzle<1, 0, 3, 2>(MVector V) { return vrev64q_f32(V); }

	template<> inline MVector      MC_VECCALL     MVectorSwizzle<0, 1, 0, 1>(MVector V) { float32x2_t vt = vget_low_f32(V); return vcombine_f32(vt, vt); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<2, 3, 2, 3>(MVector V) { float32x2_t vt = vget_high_f32(V); return vcombine_f32(vt, vt); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<1, 0, 1, 0>(MVector V) { float32x2_t vt = vrev64_f32(vget_low_f32(V)); return vcombine_f32(vt, vt); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<3, 2, 3, 2>(MVector V) { float32x2_t vt = vrev64_f32(vget_high_f32(V)); return vcombine_f32(vt, vt); }

	template<> inline MVector      MC_VECCALL     MVectorSwizzle<0, 1, 3, 2>(MVector V) { return vcombine_f32(vget_low_f32(V), vrev64_f32(vget_high_f32(V))); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<1, 0, 2, 3>(MVector V) { return vcombine_f32(vrev64_f32(vget_low_f32(V)), vget_high_f32(V)); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<2, 3, 1, 0>(MVector V) { return vcombine_f32(vget_high_f32(V), vrev64_f32(vget_low_f32(V))); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<3, 2, 0, 1>(MVector V) { return vcombine_f32(vrev64_f32(vget_high_f32(V)), vget_low_f32(V)); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<3, 2, 1, 0>(MVector V) { return vcombine_f32(vrev64_f32(vget_high_f32(V)), vrev64_f32(vget_low_f32(V))); }

	template<> inline MVector      MC_VECCALL     MVectorSwizzle<0, 0, 2, 2>(MVector V) { return vtrnq_f32(V, V).val[0]; }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<1, 1, 3, 3>(MVector V) { return vtrnq_f32(V, V).val[1]; }

	template<> inline MVector      MC_VECCALL     MVectorSwizzle<0, 0, 1, 1>(MVector V) { return vzipq_f32(V, V).val[0]; }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<2, 2, 3, 3>(MVector V) { return vzipq_f32(V, V).val[1]; }

	template<> inline MVector      MC_VECCALL     MVectorSwizzle<0, 2, 0, 2>(MVector V) { return vuzpq_f32(V, V).val[0]; }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<1, 3, 1, 3>(MVector V) { return vuzpq_f32(V, V).val[1]; }

	template<> inline MVector      MC_VECCALL     MVectorSwizzle<1, 2, 3, 0>(MVector V) { return vextq_f32(V, V, 1); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<2, 3, 0, 1>(MVector V) { return vextq_f32(V, V, 2); }
	template<> inline MVector      MC_VECCALL     MVectorSwizzle<3, 0, 1, 2>(MVector V) { return vextq_f32(V, V, 3); }

#endif // _XM_ARM_NEON_INTRINSICS_ && !_XM_NO_INTRINSICS_

}