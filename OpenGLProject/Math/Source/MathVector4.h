#pragma once

namespace MadCanyon
{
	class Vector3;
	class Matrix44;
	class Vector4 : public DirectX::XMFLOAT4
	{
	public:
		// コンストラクタで0埋め
		Vector4()
		{
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		// 座標指定付きコンストラクタ
		Vector4(float _x, float _y, float _z, float _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}

		// XMVECTORから代入してきた時
		Vector4(const MVector& V) { MVecStore(this, V); }
		// XMFLOAT4から代入してきた時
		Vector4(const DirectX::XMFLOAT4& V)
		{
			x = V.x;
			y = V.y;
			z = V.z;
			w = V.w;
		}

		// XMVECTORへ変換
		operator MVector() const { return MLoadVec4(this); }
		// Math::Vector3と互換性を持つための関数
		operator DirectX::SimpleMath::Vector4& () { return *(DirectX::SimpleMath::Vector4*)this; }

		// 演算子
		Vector4& operator+= (const Vector4& v)
		{
			*this = Vec4Add(Vec4Set(*this), Vec4Set(v));
			return *this;
		}
		Vector4& operator-= (const Vector4& v)
		{
			*this = Vec4Sub(Vec4Set(*this), Vec4Set(v));
			return *this;
		}
		Vector4& operator*= (const Vector4& v)
		{
			*this = Vec4Mul(Vec4Set(*this), Vec4Set(v));
			return *this;
		}
		Vector4& operator*= (float s)
		{
			*this = Vec4Mul(Vec4Set(*this), _mm_set_ps1(s));
			return *this;
		}
		Vector4& operator/= (float s)
		{
			assert(s != 0.0f);
			*this = Vec4Mul(Vec4Set(*this), _mm_set_ps1(1.f / s));
			return *this;
		}

		Vector4 operator+ (const Vector4& v)const { return Vec4Add(Vec4Set(*this), Vec4Set(v)); }
		Vector4 operator- (const Vector4& v)const { return Vec4Sub(Vec4Set(*this), Vec4Set(v)); }
		Vector4 operator* (const Vector4& v)const { return Vec4Mul(Vec4Set(*this), Vec4Set(v)); }
		Vector4 operator/ (const Vector4& v)const { return Vec4Div(Vec4Set(*this), Vec4Set(v)); }
		Vector4 operator* (float s)const { return Vec4Mul(Vec4Set(*this), Vec4Set(s)); }
		Vector4 operator/ (float s)const
		{
			assert(s != 0.0f);
			return Vec4Mul(Vec4Set(*this), _mm_set_ps1(1.f / s));
		}

		// セット
		void Set(float fx, float fy, float fz, float fw)
		{
			x = fx;
			y = fy;
			z = fz;
			w = fw;
		}

		void Set(const Vector3& v3, float fw)
		{
			x = v3.x;
			y = v3.y;
			z = v3.z;
			w = fw;
		}

		// 長さ
		float Length() const;
		// 長さの２乗(高速なので判定用に使用することが多い)
		float LengthSquared() const;

		// 内積
		float Dot(const Vector4& V) const;
		// 内積
		static float sDot(const Vector4& v1, const Vector4& v2);

		// 外積
		Vector4 Cross(const Vector4& v1, Vector4& v2)const;
		// 外積
		static Vector4 sCross(const Vector4& v1, Vector4& v2, const Vector4& v3);

		// 自分を正規化
		Vector4& Normalize();

		// クランプ
		Vector4& Clamp(const Vector4& minVec, const Vector4& maxVec);

		// 線形補間
		Vector4& Lerp(const Vector4& startVec, const Vector4& endVec, float time);

		// スムーズステップ補間
		Vector4& SmoothStep(const Vector4& v1, const Vector4& v2, float t);

		// Catmull-Romスプライン補間
		Vector4& CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t);

		// エルミネートスプライン補間
		Vector4& Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t);

		// 変換
		Vector4& Transform(const Matrix44& m);

		// 配列化
		std::vector<float> ToArray() const
		{
			return { x, y, z, w };
		}

		//===============静的変数================
		/**
		* @brief 全て0
		*/
		static const Vector4 Zero;
		/**
		* @brief 全て1
		*/
		static const Vector4 One;

		//===============静的関数================

		// float配列の先頭アドレスとして返す
		operator const float* () const
		{
			return &x;
		}
		operator float* ()
		{
			return &x;
		}
	};
	inline MVector MC_VECCALL  Vec4Set(const Vector4& v)
	{
		return _mm_set_ps(v.w, v.z, v.y, v.x);
	}
	inline MVector MC_VECCALL  Vec4Set(float x, float y, float z, float w)
	{
		return _mm_set_ps(w, z, y, x);
	}
	inline MVector MC_VECCALL  Vec4Set(float value)
	{
		return _mm_set_ps1(value);
	}
	inline void MC_VECCALL  MVecStore(Vector4* out, const MVector& in)
	{
		_mm_storeu_ps(&out->x, in);
	}
	inline MVector MC_VECCALL  MLoadVec4(const Vector4* in)
	{
		return _mm_loadu_ps(&in->x);
	}
	inline MVector MC_VECCALL  Vec4Add(MVector left, MVector right)
	{
		return _mm_add_ps(left, right);
	}
	inline MVector MC_VECCALL  Vec4Sub(MVector left, MVector right)
	{
		return _mm_sub_ps(left, right);
	}
	inline MVector MC_VECCALL  Vec4Mul(MVector left, MVector right)
	{
		return _mm_mul_ps(left, right);
	}
	inline MVector MC_VECCALL  Vec4Div(MVector left, MVector right)
	{
		return _mm_div_ps(left, right);
	}
	inline MVector MC_VECCALL Vec4Length(MVector v)
	{
		// Perform the dot product on x,y,z and w
		MVector vLengthSq = _mm_mul_ps(v, v);
		// vTemp has z and w
		MVector vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
		// x+z, y+w
		vLengthSq = _mm_add_ps(vLengthSq, vTemp);
		// x+z,x+z,x+z,y+w
		vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
		// ??,??,y+w,y+w
		vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
		// ??,??,x+z+y+w,??
		vLengthSq = _mm_add_ps(vLengthSq, vTemp);
		// Splat the length
		vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
		// Get the length
		vLengthSq = _mm_sqrt_ps(vLengthSq);
		return vLengthSq;
	}
	inline MVector MC_VECCALL Vec4Dot(MVector left, MVector right)
	{
		__m128 temp2 = right;
		//*this * temp2.
		__m128 temp = _mm_mul_ps(left, temp2);
		//// XをZ位置に、YをW位置にコピー.
		temp2 = _mm_shuffle_ps(temp2, temp, _MM_SHUFFLE(1, 0, 0, 0));
		// 加算 Z = X+Z; W = Y+W;
		temp2 = _mm_add_ps(temp2, temp);
		// WをZ位置にコピーします
		temp = _mm_shuffle_ps(temp, temp2, _MM_SHUFFLE(0, 3, 0, 0));
		// Add Z and W together
		temp = _mm_add_ps(temp, temp2);
		// Z　return
		return _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(2, 2, 2, 2));
	}
	inline MVector MC_VECCALL Vec4Cross(MVector axisNorm, MVector left, MVector right)
	{
		// left(zwyz) * right(wzwy)
		__m128 result = _mm_shuffle_ps(left, left, _MM_SHUFFLE(2, 1, 3, 2));
		__m128 temp3 = _mm_shuffle_ps(right, right, _MM_SHUFFLE(1, 3, 2, 3));
		result = _mm_mul_ps(result, temp3);
		// - left(wzwy) * right(zwyz)
		__m128 temp2 = _mm_shuffle_ps(left, left, _MM_SHUFFLE(1, 3, 2, 3));
		temp3 = _mm_shuffle_ps(temp3, temp3, _MM_SHUFFLE(1, 3, 0, 1));
		temp2 = _mm_mul_ps(temp2, temp3);
		result = _mm_sub_ps(result, temp2);
		// term1 * this(yxxx)
		__m128 temp1 = _mm_shuffle_ps(axisNorm, axisNorm, _MM_SHUFFLE(0, 0, 0, 1));
		result = _mm_mul_ps(result, temp1);

		// left(ywxz) * right(wxwx)
		temp2 = _mm_shuffle_ps(left, left, _MM_SHUFFLE(2, 0, 3, 1));
		temp3 = _mm_shuffle_ps(right, right, _MM_SHUFFLE(0, 3, 0, 3));
		temp3 = _mm_mul_ps(temp3, temp2);
		// - left(wxwx) * right(ywxz)
		temp2 = _mm_shuffle_ps(temp2, temp2, _MM_SHUFFLE(2, 1, 2, 1));
		temp1 = _mm_shuffle_ps(right, right, _MM_SHUFFLE(2, 0, 3, 1));
		temp2 = _mm_mul_ps(temp2, temp1);
		temp3 = _mm_sub_ps(temp3, temp2);
		// result - temp * this(zzyy)
		temp1 = _mm_shuffle_ps(axisNorm, axisNorm, _MM_SHUFFLE(1, 1, 2, 2));
		temp1 = _mm_mul_ps(temp1, temp3);
		result = _mm_sub_ps(result, temp1);

		// left(yzxy) * right(zxyx)
		temp2 = _mm_shuffle_ps(left, left, _MM_SHUFFLE(1, 0, 2, 1));
		temp3 = _mm_shuffle_ps(right, right, _MM_SHUFFLE(0, 1, 0, 2));
		temp3 = _mm_mul_ps(temp3, temp2);
		// - left(zxyx) * right(yzxy)
		temp2 = _mm_shuffle_ps(temp2, temp2, _MM_SHUFFLE(2, 0, 2, 1));
		temp1 = _mm_shuffle_ps(right, right, _MM_SHUFFLE(1, 0, 2, 1));
		temp1 = _mm_mul_ps(temp1, temp2);
		temp3 = _mm_sub_ps(temp3, temp1);
		// result + term * this(wwwz)
		temp1 = _mm_shuffle_ps(axisNorm, axisNorm, _MM_SHUFFLE(2, 3, 3, 3));
		temp3 = _mm_mul_ps(temp3, temp1);
		result = _mm_add_ps(result, temp3);
		return result;
	}
	inline MVector MC_VECCALL Vec4Normalize(MVector V)
	{
		//x,y,z,wのドット積.
		__m128 lengthSq = _mm_mul_ps(V, V);
		// temp = { lengthSq.z, lengthSq.w, lengthSq.z, lengthSq.w }
		__m128 temp = XM_PERMUTE_PS(lengthSq, _MM_SHUFFLE(3, 2, 3, 2));
		// x+z, y+w
		lengthSq = _mm_add_ps(lengthSq, temp);
		// x+z,x+z,x+z,y+w
		lengthSq = XM_PERMUTE_PS(lengthSq, _MM_SHUFFLE(1, 0, 0, 0));
		// ??,??,y+w,y+w
		temp = _mm_shuffle_ps(temp, lengthSq, _MM_SHUFFLE(3, 3, 0, 0));
		// ??,??,x+z+y+w,??
		lengthSq = _mm_add_ps(lengthSq, temp);
		// lengthSq = { lengthSq.z, lengthSq.z, lengthSq.z, lengthSq.z}
		lengthSq = XM_PERMUTE_PS(lengthSq, _MM_SHUFFLE(2, 2, 2, 2));

		//lengthSqの平方根.
		__m128 result = _mm_sqrt_ps(lengthSq);

		//全ての要素を0.
		__m128 zeroMask = _mm_setzero_ps();
		// Test for a divide by zero (Must be FP to detect -0.0)
		zeroMask = _mm_cmpneq_ps(zeroMask, result);
		// Failsafe on zero (Or epsilon) length planes
		// If the length is infinity, set the elements to zero
		lengthSq = _mm_cmpneq_ps(lengthSq, Simd::MV_Infinity);

		//正規化を行うため0で除算
		result = _mm_div_ps(V, result);
		//どれかの値がInifinityの場合0にします 
		result = _mm_and_ps(result, zeroMask);

		// Infinityの長さに基づいてQNaNまたは結果を選択
		__m128 temp1 = _mm_andnot_ps(lengthSq, Simd::MV_QNaN);
		__m128 temp2 = _mm_and_ps(result, lengthSq);

		result = _mm_or_ps(temp1, temp2);
		return result;


	}
	inline MVector MC_VECCALL Vec4Transform(MVector V, MMATRIX M)
	{
		// Splat x,y,z and w
		MVector tempX = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
		MVector tempY = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
		MVector tempZ = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
		MVector tempW = XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
		// Mul by the matrix
		tempX = _mm_mul_ps(tempX, M.r[0]);
		tempY = _mm_mul_ps(tempY, M.r[1]);
		tempZ = _mm_mul_ps(tempZ, M.r[2]);
		tempW = _mm_mul_ps(tempW, M.r[3]);
		// Add them all together
		tempX = _mm_add_ps(tempX, tempY);
		tempZ = _mm_add_ps(tempZ, tempW);
		tempX = _mm_add_ps(tempX, tempZ);
		return tempX;
	}
	inline MVector MC_VECCALL Vec4CatmullRom(MVector v1, MVector v2, MVector v3, MVector v4, float time)
	{
		float t2 = time * time;
		float t3 = time * t2;

		MVector p0 = _mm_set_ps1((-t3 + 2.0f * t2 - time) * 0.5f);
		MVector p1 = _mm_set_ps1((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
		MVector p2 = _mm_set_ps1((-3.0f * t3 + 4.0f * t2 + time) * 0.5f);
		MVector p3 = _mm_set_ps1((t3 - t2) * 0.5f);

		p0 = _mm_mul_ps(p0, v1);
		p1 = _mm_mul_ps(p1, v2);
		p2 = _mm_mul_ps(p2, v3);
		p3 = _mm_mul_ps(p3, v4);
		p0 = _mm_add_ps(p0, p1);
		p2 = _mm_add_ps(p2, p3);
		p0 = _mm_add_ps(p0, p2);

		return p0;
	}
	inline MVector MC_VECCALL Vec4Hermite(MVector posVec1, MVector tangentVec1, MVector posVec2, MVector tangentVec2, float time)
	{
		float t2 = time * time;
		float t3 = time * t2;

		MVector p0 = _mm_set_ps1(2.0f * t3 - 3.0f * t2 + 1.0f);
		MVector t0 = _mm_set_ps1(t3 - 2.0f * t2 + time);
		MVector p1 = _mm_set_ps1(-2.0f * t3 + 3.0f * t2);
		MVector t1 = _mm_set_ps1(t3 - t2);

		MVector result = _mm_mul_ps(p0, posVec1);
		MVector temp = _mm_mul_ps(t0, tangentVec1);
		result = _mm_add_ps(result, temp);
		temp = _mm_mul_ps(p1, posVec2);
		result = _mm_add_ps(result, temp);
		temp = _mm_mul_ps(t1, tangentVec2);
		result = _mm_add_ps(result, temp);

		return result;
	}
}