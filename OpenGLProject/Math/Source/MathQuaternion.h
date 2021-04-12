#pragma once

namespace MadCanyon
{
	class Vector3;
	class Matrix44;
	class Quaternion : public DirectX::XMFLOAT4
	{
	public:

		// コンストラクタで0埋め
		Quaternion()
		{
			x = 0;
			y = 0;
			z = 0;
			w = 1;
		}
		// 座標指定付きコンストラクタ
		Quaternion(float _x, float _y, float _z, float _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}

		Quaternion(const Vector3& vec3, float _w)
		{
			x = vec3.x;
			y = vec3.y;
			z = vec3.z;
			w = _w;
		}

		// XMVECTORから代入してきた時
		Quaternion(const MVector& V)
		{
			MQuatStore(this, V);
		}
		// XMFLOAT3から代入してきた時
		Quaternion(const DirectX::XMFLOAT4& V)
		{
			x = V.x;
			y = V.y;
			z = V.z;
			w = V.w;
		}

		// XMVECTORへ変換
		operator MVector() const { return MLoadQuat(this); }
		// Math::Quaternionと互換性を持つための関数
		operator DirectX::SimpleMath::Quaternion& () { return *(DirectX::SimpleMath::Quaternion*)this; }

		// 演算子
		Quaternion& operator*= (const Quaternion& q)
		{
			*this = QuatMul(QuatSet(*this), QuatSet(q));
			return *this;
		}

		Quaternion operator* (const Quaternion& q) const { return QuatMul(QuatSet(*this), QuatSet(q)); }

		Quaternion& operator/= (const Quaternion& q)
		{
			*this = QuatMul(QuatSet(*this), QuaternionInverse(QuatSet(q)));
			return *this;
		}

		Quaternion operator/ (const Quaternion& q)const{ return QuatMul(QuatSet(*this), QuaternionInverse(QuatSet(q))); }

		//=================================
		//
		// 作成
		//
		//=================================

		void Set(const Vector3& vec3,float _w)
		{
			x = vec3.x;
			y = vec3.y;
			z = vec3.z;
			w = _w;
		}

		/**
		* @brief aAxis軸をaAngleRad回転させるクォータニオンを生成します
		*/
		Quaternion& CreateAngleAxis(float aAngeRadian, const Vector3& aAxisNorm);

		// 指定軸回転クォータニオン作成
		Quaternion& CreateRotationAxis(const Vector3& axis, float angle);

		// x,y,z回転角度からクォータニオン作成
		Quaternion& CreateRotationFromAngles(float angleX, float angleY, float angleZ);

		// x,y,z回転角度からクォータニオン作成
		Quaternion& CreateRotationFromAngles(const Vector3& angle);

		// 行列から作成
		Quaternion& CreateFromMatrix(const Matrix44& m);

		//=================================
		//
		// 操作
		//
		//=================================

		Quaternion& SetIdentity();

		// 共役化
		// (小ネタ)クォータニオンの長さが１の場合は、逆クォータニオンと等しくなる
		Quaternion& Conjugate();

		// thisを逆クォータニオン化
		Quaternion& Inverse();

		// 逆クォータニオンを返します
		Quaternion Inversed()const { return Quaternion{ *this }.Inverse(); }

		// thisを正規化
		Quaternion& Normalize();

		// 正規化したクォータニオンを返します
		Quaternion Normalized()const { return Quaternion{ *this }.Normalize(); }

		// 球面線形補間
		Quaternion& Slerp(const Quaternion& q1, const Quaternion& q2, float t);

		// Z軸を指定方向に向ける
		Quaternion& LookTo(const Vector3& dir, const Vector3& up);

		/**
		* @brief クォータニオン間の角度(0.0f～PI)を返します
		* @note	クォータニオンで使われる角度(通常の半分)ではありません
		*/
		float Angle(const Quaternion& other)const;

		/**
		* @brief 内積を返します
		*/
		float Dot(const Quaternion& other)const { return (w * other.w) + (x * other.x) + (y * other.y) + (z * other.z); }


		//=================================
		//
		// プロパティ
		//
		//=================================

		// クォータニオンの大きさ
		float Length() const;

		// 合成
		Quaternion& Multiply(const Quaternion& other)
		{
			*this *= other;
			return *this;
		}

		// 合成
		static Quaternion Multiply(const Quaternion& leftQuat, const Quaternion& rightQuat);

		/**
		* @brief (無回転(Vector3{0.0f,0.0f,1.0f})から)aLookNormへ回転させるクォータニオンを生成します
		* @param aWorldUp 回転後の頭の向き(Y軸方向)
		*/
		Quaternion& CreateLookRotation(const Vector3& lookNorm, const Vector3& worldUp = Vector3::Up);

		// 配列化
		std::vector<float> ToArray() const
		{
			return { x, y, z, w };
		}

		// X軸取得
		Vector3 Right()const
		{
			return {
				1 - 2 * y * y - 2 * z * z,
				2 * x * y + 2 * w * z,
				2 * x * z - 2 * w * y
			};
		}

		// Y軸取得
		Vector3 Up()const
		{
			return {
				2 * x * y - 2 * w * z,
				1 - 2 * x * x - 2 * z * z,
				2 * y * z + 2 * w * x
			};
		}

		// Z軸取得
		Vector3 Forward()const
		{
			return {
				2 * x * z + 2 * w * y,
				2 * y * z - 2 * w * x,
				1 - 2 * x * x - 2 * y * y
			};
		}

		// float配列の先頭アドレスとして返す
		operator const float* () const
		{
			return &x;
		}
		operator float* ()
		{
			return &x;
		}

		Vector3 MultiplyPoint(const Vector3& vec)const;

		//===============静的変数================
		/**
		* @brief 単位クォータニオン
		*/
		static const Quaternion Identity;
	};
	inline MVector MC_VECCALL  QuatSet(const Quaternion& v)
	{
		return _mm_set_ps(v.w, v.z, v.y, v.x);
	}
	inline MVector MC_VECCALL  QuatSet(float x, float y, float z, float w)
	{
		return _mm_set_ps(w, z, y, x);
	}
	inline MVector MC_VECCALL  QuatSet(float value)
	{
		return _mm_set_ps1(value);
	}
	inline void MC_VECCALL  MQuatStore(Quaternion* out, const MVector& in)
	{
		_mm_storeu_ps(&out->x, in);
	}
	inline MVector MC_VECCALL  MLoadQuat(const Quaternion* in)
	{
		return _mm_loadu_ps(&in->x);
	}
	inline MVector MC_VECCALL  QuatAdd(MVector left, MVector right)
	{
		return _mm_add_ps(left, right);
	}
	inline MVector MC_VECCALL  QuatSub(MVector left, MVector right)
	{
		return _mm_sub_ps(left, right);
	}
	inline MVector MC_VECCALL  QuatVecMul(MVector left, MVector right)
	{
		return _mm_mul_ps(left, right);
	}
	inline MVector MC_VECCALL  QuatMul(MVector left, MVector right)
	{
		// [ (Q2.w * Q1.x) + (Q2.x * Q1.w) + (Q2.y * Q1.z) - (Q2.z * Q1.y),
		//   (Q2.w * Q1.y) - (Q2.x * Q1.z) + (Q2.y * Q1.w) + (Q2.z * Q1.x),
		//   (Q2.w * Q1.z) + (Q2.x * Q1.y) - (Q2.y * Q1.x) + (Q2.z * Q1.w),
		//   (Q2.w * Q1.w) - (Q2.x * Q1.x) - (Q2.y * Q1.y) - (Q2.z * Q1.z) ]

		static const MVector ControlWZYX = { 1.0f, -1.0f, 1.0f, -1.0f };
		static const MVector ControlZWXY = { 1.0f, 1.0f, -1.0f, -1.0f };
		static const MVector ControlYXWZ = { -1.0f, 1.0f, 1.0f, -1.0f };

		MVector otherX = right;
		MVector otherY = right;
		MVector otherZ = right;
		MVector result = right;
		//vResult = { vResult.mW, vResult.mW, vResult.mW, vResult.mW }.
		result = _mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 3, 3, 3));
		//otherX = { otherX.x, otherX.x, otherX.x, otherX.x }.
		otherX = _mm_shuffle_ps(otherX, otherX, _MM_SHUFFLE(0, 0, 0, 0));
		//otherY = { otherY.y, otherY.y, otherY.y, otherY.y }.
		otherY = _mm_shuffle_ps(otherY, otherY, _MM_SHUFFLE(1, 1, 1, 1));
		//otherZ = { otherZ.z, otherZ.z, otherZ.z, otherZ.z }.
		otherZ = _mm_shuffle_ps(otherZ, otherZ, _MM_SHUFFLE(2, 2, 2, 2));

		//result * left.
		result = Vec4Mul(result, QuatSet(left));

		MVector ownShuffle = QuatSet(left);

		//ownShuffle = { ownShuffle.mW, ownShuffle.z, ownShuffle.y, ownShuffle.x }.
		ownShuffle = _mm_shuffle_ps(ownShuffle, ownShuffle, _MM_SHUFFLE(0, 1, 2, 3));

		// otherX * ownShuffle(thisWZYX)
		otherX = QuatVecMul(otherX, ownShuffle);
		//ownShuffle = { ownShuffle.z, ownShuffle.mW, ownShuffle.x, ownShuffle.y }.
		ownShuffle = _mm_shuffle_ps(ownShuffle, ownShuffle, _MM_SHUFFLE(2, 3, 0, 1));

		// YとZの符号を反転
		otherX = QuatVecMul(otherX, ControlWZYX);

		// otherY * ownShuffle(thisZWXY)
		otherY = QuatVecMul(otherY, ownShuffle);
		//ownShuffle = { ownShuffle.y, ownShuffle.x, ownShuffle.mW, ownShuffle.z }.
		ownShuffle = _mm_shuffle_ps(ownShuffle, ownShuffle, _MM_SHUFFLE(0, 1, 2, 3));

		// ZとWの符号を反転
		otherY = QuatVecMul(otherY, ControlZWXY);

		// otherZ * ownShuffle(thisYXWZ)
		otherZ = QuatVecMul(otherZ, ownShuffle);
		result = QuatAdd(result, otherX);

		// XとWの符号を反転
		otherZ = QuatVecMul(otherZ, ControlYXWZ);
		// otherY + otherZ
		otherY = QuatAdd(otherY, otherZ);
		// result + otherY
		result = QuatAdd(result, otherY);

		return result;
	}
	inline MVector MC_VECCALL  QuatDiv(MVector left, MVector right)
	{
		return _mm_div_ps(left, right);
	}
	inline MVector MC_VECCALL  QuatLength(MVector left, MVector right)
	{
		// left * left
		__m128 lengthSq = QuatVecMul(left, left);
		// temp = {lengthSq.z,lengthSq.w,lengthSq.z,lengthSq.w}
		__m128 temp = XM_PERMUTE_PS(lengthSq, _MM_SHUFFLE(3, 2, 3, 2));
		// x+z, y+w
		lengthSq = _mm_add_ps(lengthSq, temp);
		// x+z,x+z,x+z,y+w
		lengthSq = XM_PERMUTE_PS(lengthSq, _MM_SHUFFLE(1, 0, 0, 0));
		// ??,??,y+w,y+w
		temp = _mm_shuffle_ps(temp, lengthSq, _MM_SHUFFLE(3, 3, 0, 0));
		// ??,??,x+z+y+w,??
		lengthSq = _mm_add_ps(lengthSq, temp);
		// lengthSq =  {lengthSq.z,lengthSq.z,lengthSq.z,lengthSq.z}
		lengthSq = XM_PERMUTE_PS(lengthSq, _MM_SHUFFLE(2, 2, 2, 2));
		//平方根
		lengthSq = _mm_sqrt_ps(lengthSq);

		return lengthSq;
	}
	inline MVector MC_VECCALL QuatSlerp(MVector left, MVector right, MVector time)
	{
		static const MVector OneMinusEpsilon = { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f };
		//static const MVECTORU32 SignMask2 = { 0x80000000, 0x00000000, 0x00000000, 0x00000000  };
		static const MVectorI SignMask2 = _mm_setr_epi32(0x80000000, 0x00000000, 0x00000000, 0x00000000);

		MVector cosOmega = Vec4Dot(left, right);

		const MVector Zero = _mm_setzero_ps();
		MVector Control = Simd::VectorLess(cosOmega, Zero);
		MVector Sign = Simd::VectorSelect(Simd::MV_One, Simd::MV_NegativeOne, Control);

		cosOmega = _mm_mul_ps(cosOmega, Sign);

		Control = Simd::VectorLess(cosOmega, OneMinusEpsilon);

		MVector sinOmega = _mm_mul_ps(cosOmega, cosOmega);
		sinOmega = _mm_sub_ps(Simd::MV_One, sinOmega);
		sinOmega = _mm_sqrt_ps(sinOmega);

		MVector omega = Simd::VectorATan2(sinOmega, cosOmega);

		MVector V01 = XM_PERMUTE_PS(time, _MM_SHUFFLE(2, 3, 0, 1));
		V01 = _mm_and_ps(V01, Simd::MV_MaskXY);
		V01 = _mm_xor_ps(V01, _mm_castsi128_ps(SignMask2));
		V01 = _mm_add_ps(Simd::MV_IdentityR0, V01);

		MVector S0 = _mm_mul_ps(V01, omega);
		S0 = Simd::VectorSin(S0);
		S0 = _mm_div_ps(S0, sinOmega);

		S0 = Simd::VectorSelect(V01, S0, Control);

		MVector S1 = _mm_shuffle_ps(S0, S0, _MM_SHUFFLE(1, 1, 1, 1));
		S0 = _mm_shuffle_ps(S0, S0, _MM_SHUFFLE(0, 0, 0, 0));

		S1 = _mm_mul_ps(S1, Sign);
		MVector Result = _mm_mul_ps(left, S0);
		S1 = _mm_mul_ps(S1, right);
		Result = _mm_add_ps(Result, S1);
		return Result;
	}
	inline MVector MC_VECCALL  QuatRotationNormal(MVector normalAxis, float angle)
	{
		MVector N = _mm_and_ps(normalAxis, Simd::MV_Mask3);
		N = _mm_or_ps(N, Simd::MV_IdentityR3);
		MVector Scale = _mm_set_ps1(0.5f * angle);
		MVector vSine;
		MVector vCosine;
		Simd::VectorSinCos(&vSine, &vCosine, Scale);
		Scale = _mm_and_ps(vSine, Simd::MV_Mask3);
		vCosine = _mm_and_ps(vCosine, Simd::MV_MaskW);
		Scale = _mm_or_ps(Scale, vCosine);
		N = _mm_mul_ps(N, Scale);
		return N;
	}
	inline MVector MC_VECCALL  QuatRotationAxis(MVector normalAxis, float angle)
	{
		return QuatRotationNormal(normalAxis, angle);
	}
	inline MVector MC_VECCALL QuatRotationRollPitchYawFromVector(
		MVector Angles // <Pitch, Yaw, Roll, 0>
	)
	{
		static const MVector  Sign = { 1.0f, -1.0f, -1.0f, 1.0f };

		MVector HalfAngles = _mm_mul_ps(Angles, Simd::MV_OneHalf.v);

		MVector SinAngles, CosAngles;
		Simd::VectorSinCos(&SinAngles, &CosAngles, HalfAngles);

		using namespace Simd;
		MVector P0 = MVectorPermute<MC_PERMUTE_0X, MC_PERMUTE_1X, MC_PERMUTE_1X, MC_PERMUTE_1X>(SinAngles, CosAngles);
		MVector Y0 = MVectorPermute<MC_PERMUTE_1Y, MC_PERMUTE_0Y, MC_PERMUTE_1Y, MC_PERMUTE_1Y>(SinAngles, CosAngles);
		MVector R0 = MVectorPermute<MC_PERMUTE_1Z, MC_PERMUTE_1Z, MC_PERMUTE_0Z, MC_PERMUTE_1Z>(SinAngles, CosAngles);
		MVector P1 = MVectorPermute<MC_PERMUTE_0X, MC_PERMUTE_1X, MC_PERMUTE_1X, MC_PERMUTE_1X>(CosAngles, SinAngles);
		MVector Y1 = MVectorPermute<MC_PERMUTE_1Y, MC_PERMUTE_0Y, MC_PERMUTE_1Y, MC_PERMUTE_1Y>(CosAngles, SinAngles);
		MVector R1 = MVectorPermute<MC_PERMUTE_1Z, MC_PERMUTE_1Z, MC_PERMUTE_0Z, MC_PERMUTE_1Z>(CosAngles, SinAngles);


		MVector Q1 = _mm_mul_ps(P1, Sign);
		MVector Q0 = _mm_mul_ps(P0, Y0);
		Q1 = _mm_mul_ps(Q1, Y1);
		Q0 = _mm_mul_ps(Q0, R0);
		MVector Q = _mm_mul_ps(Q1, R1);
		Q = _mm_add_ps(Q, Q0);
		return Q;
	}
	inline MVector MC_VECCALL QuatRotationRollPitchYaw(float Pitch, float Yaw, float Roll)
	{
		MVector Angles = _mm_set_ps(0.0f, Roll, Yaw, Pitch);
		MVector Q = QuatRotationRollPitchYawFromVector(Angles);
		return Q;
	}
	inline MVector MC_VECCALL QuatConjugate(MVector v)
	{
		static const MVector NegativeOne3 = { -1.0f, -1.0f, -1.0f, 1.0f };
		return _mm_mul_ps(v, NegativeOne3);
	}
	inline MVector MC_VECCALL QuaternionRotationMatrix(MMATRIX M)
	{
		static const MVECTORF32 XMPMMP = { +1.0f, -1.0f, -1.0f, +1.0f };
		static const MVECTORF32 XMMPMP = { -1.0f, +1.0f, -1.0f, +1.0f };
		static const MVECTORF32 XMMMPP = { -1.0f, -1.0f, +1.0f, +1.0f };

		MVector r0 = M.r[0];  // (r00, r01, r02, 0)
		MVector r1 = M.r[1];  // (r10, r11, r12, 0)
		MVector r2 = M.r[2];  // (r20, r21, r22, 0)

		// (r00, r00, r00, r00)
		MVector r00 = XM_PERMUTE_PS(r0, _MM_SHUFFLE(0, 0, 0, 0));
		// (r11, r11, r11, r11)
		MVector r11 = XM_PERMUTE_PS(r1, _MM_SHUFFLE(1, 1, 1, 1));
		// (r22, r22, r22, r22)
		MVector r22 = XM_PERMUTE_PS(r2, _MM_SHUFFLE(2, 2, 2, 2));

		// x^2 >= y^2 equivalent to r11 - r00 <= 0
		// (r11 - r00, r11 - r00, r11 - r00, r11 - r00)
		MVector r11mr00 = _mm_sub_ps(r11, r00);
		MVector x2gey2 = _mm_cmple_ps(r11mr00, Simd::MV_Zero);

		// z^2 >= w^2 equivalent to r11 + r00 <= 0
		// (r11 + r00, r11 + r00, r11 + r00, r11 + r00)
		MVector r11pr00 = _mm_add_ps(r11, r00);
		MVector z2gew2 = _mm_cmple_ps(r11pr00, Simd::MV_Zero);

		// x^2 + y^2 >= z^2 + w^2 equivalent to r22 <= 0
		MVector x2py2gez2pw2 = _mm_cmple_ps(r22, Simd::MV_Zero);

		// (+r00, -r00, -r00, +r00)
		MVector t0 = _mm_mul_ps(XMPMMP, r00);

		// (-r11, +r11, -r11, +r11)
		MVector t1 = _mm_mul_ps(XMMPMP, r11);

		// (-r22, -r22, +r22, +r22)
		MVector t2 = _mm_mul_ps(XMMMPP, r22);

		// (4*x^2, 4*y^2, 4*z^2, 4*w^2)
		MVector x2y2z2w2 = _mm_add_ps(t0, t1);
		x2y2z2w2 = _mm_add_ps(t2, x2y2z2w2);
		x2y2z2w2 = _mm_add_ps(x2y2z2w2, Simd::MV_One);

		// (r01, r02, r12, r11)
		t0 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 2, 2, 1));
		// (r10, r10, r20, r21)
		t1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1, 0, 0, 0));
		// (r10, r20, r21, r10)
		t1 = XM_PERMUTE_PS(t1, _MM_SHUFFLE(1, 3, 2, 0));
		// (4*x*y, 4*x*z, 4*y*z, unused)
		MVector xyxzyz = _mm_add_ps(t0, t1);

		// (r21, r20, r10, r10)
		t0 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(0, 0, 0, 1));
		// (r12, r12, r02, r01)
		t1 = _mm_shuffle_ps(r1, r0, _MM_SHUFFLE(1, 2, 2, 2));
		// (r12, r02, r01, r12)
		t1 = XM_PERMUTE_PS(t1, _MM_SHUFFLE(1, 3, 2, 0));
		// (4*x*w, 4*y*w, 4*z*w, unused)
		MVector xwywzw = _mm_sub_ps(t0, t1);
		xwywzw = _mm_mul_ps(XMMPMP, xwywzw);

		// (4*x^2, 4*y^2, 4*x*y, unused)
		t0 = _mm_shuffle_ps(x2y2z2w2, xyxzyz, _MM_SHUFFLE(0, 0, 1, 0));
		// (4*z^2, 4*w^2, 4*z*w, unused)
		t1 = _mm_shuffle_ps(x2y2z2w2, xwywzw, _MM_SHUFFLE(0, 2, 3, 2));
		// (4*x*z, 4*y*z, 4*x*w, 4*y*w)
		t2 = _mm_shuffle_ps(xyxzyz, xwywzw, _MM_SHUFFLE(1, 0, 2, 1));

		// (4*x*x, 4*x*y, 4*x*z, 4*x*w)
		MVector tensor0 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(2, 0, 2, 0));
		// (4*y*x, 4*y*y, 4*y*z, 4*y*w)
		MVector tensor1 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(3, 1, 1, 2));
		// (4*z*x, 4*z*y, 4*z*z, 4*z*w)
		MVector tensor2 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(2, 0, 1, 0));
		// (4*w*x, 4*w*y, 4*w*z, 4*w*w)
		MVector tensor3 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(1, 2, 3, 2));

		//最大のテンソル積行列の行を選択
		t0 = _mm_and_ps(x2gey2, tensor0);
		t1 = _mm_andnot_ps(x2gey2, tensor1);
		t0 = _mm_or_ps(t0, t1);
		t1 = _mm_and_ps(z2gew2, tensor2);
		t2 = _mm_andnot_ps(z2gew2, tensor3);
		t1 = _mm_or_ps(t1, t2);
		t0 = _mm_and_ps(x2py2gez2pw2, t0);
		t1 = _mm_andnot_ps(x2py2gez2pw2, t1);
		t2 = _mm_or_ps(t0, t1);


		//行を正規化します。ゼロによる除算は不可能
		//クォータニオンは単位長（行はゼロ以外の倍数）
		//クォータニオン）。
		t0 = Vec4Length(t2);
		return _mm_div_ps(t2, t0);

	}
	inline MVector MC_VECCALL QuaternionInverse(MVector Q)
	{
		const MVector  Zero = _mm_setzero_ps();

		//MVector L = XMVector4LengthSq(Q);
		MVector L = Vec4Dot(Q, Q);
		MVector Conjugate = QuatConjugate(Q);

		MVector Control = Simd::VectorLessOrEqual(L, Simd::MV_Epsilon.v);

		MVector Result = _mm_div_ps(Conjugate, L);

		Result = Simd::VectorSelect(Result, Zero, Control);

		return Result;
	}
	inline MVector MC_VECCALL QuaternionNormalize(MVector Q)
	{
		return Vec4Normalize(Q);
	}
}