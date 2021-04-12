#pragma once

namespace MadCanyon
{
	class Matrix44;
	class Vector2 : public DirectX::XMFLOAT2
	{
	public:
		// コンストラクタで0埋め
		Vector2()
		{
			x = 0;
			y = 0;
		}
		// 座標指定付きコンストラクタ
		Vector2(float _x, float _y)
		{
			x = _x;
			y = _y;
		}

		// XMVECTORから代入してきた時
		Vector2(const MVector& V) { MVecStore(this, V); }

		// XMFLOAT2から代入してきた時
		Vector2(const DirectX::XMFLOAT2& V)
		{
			x = V.x;
			y = V.y;
		}

		// XMVECTORへ変換
		operator MVector() const { return MLoadVec2(this); }
		// Math::Vector2と互換性を持つための関数
		operator DirectX::SimpleMath::Vector2& () { return *(DirectX::SimpleMath::Vector2*)this; }

		// 演算子
		Vector2& operator+= (const Vector2& v)
		{
			*this = Vec2Add(Vec2Set(*this), Vec2Set(v));
			return *this;
		}
		Vector2& operator-= (const Vector2& v)
		{
			*this = Vec2Sub(Vec2Set(*this), Vec2Set(v));
			return *this;
		}
		Vector2& operator*= (const Vector2& v)
		{
			*this = Vec2Mul(Vec2Set(*this), Vec2Set(v));
			return *this;
		}
		Vector2& operator*= (float s)
		{
			*this = Vec2Mul(Vec2Set(*this), _mm_set_ps1(s));
			return *this;
		}
		Vector2& operator/= (float s)
		{
			assert(s != 0.0f);
			*this = Vec2Mul(Vec2Set(*this), _mm_set_ps1(1.f / s));
			return *this;
		}

		Vector2 operator+ (const Vector2& v)const { return Vec2Add(Vec2Set(*this), Vec2Set(v)); }
		Vector2 operator- (const Vector2& v)const { return Vec2Sub(Vec2Set(*this), Vec2Set(v)); }
		Vector2 operator* (const Vector2& v)const { return Vec2Mul(Vec2Set(*this), Vec2Set(v)); }
		Vector2 operator* (float s)const { return Vec2Mul(Vec2Set(*this), _mm_set_ps1(s)); }
		Vector2 operator/ (float s)const
		{
			assert(s != 0.0f);
			return Vec2Mul(Vec2Set(*this), _mm_set_ps1(1.f / s));
		}

		// セット
		void Set(float fx, float fy) { x = fx; y = fy; }

		// 長さ
		float Length() const;
		// 長さの２乗(高速なので判定用に使用することが多い)
		float LengthSquared() const;

		// 内積
		float Dot(const Vector2& v) const;
		// 内積
		static float sDot(const Vector2& v1, const Vector2& v2);

		// 外積
		Vector2 Cross(const Vector2& rightOther)const;
		// 外積
		static Vector2 sCross(const Vector2& v1, Vector2& v2);

		// 自分を正規化
		Vector2& Normalize();

		Vector2 Normalized()const { return Vector2{ *this }.Normalize(); }

		// クランプ
		Vector2& Clamp(const Vector2& minVec, const Vector2& maxVec);

		// 線形補間
		Vector2& Lerp(const Vector2& startVec, const Vector2& endVec, float time);

		// スムーズステップ補間
		Vector2& SmoothStep(const Vector2& startVec, const Vector2& endVec, float time);

		// Catmull-Romスプライン補間
		Vector2& CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float time);

		// エルミートスプライン補間
		Vector2& Hermite(const Vector2& posVec1, const Vector2& tangentVec1, const Vector2& posVec2, const Vector2& tangentVec2, float time);

		// 行列で変換し、w=1に投影する。
		Vector2& TransformCoord(const Matrix44& m);

		// (x,y,0,0)で変換
		Vector2& TransformNormal(const Matrix44& m);

		// 配列化
		Vector<float> ToArray() const
		{
			return { x, y };
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

		//======静的変数========
		/**
		*@brief{1.0f,0.0f}
		*/
		static const Vector2 Right;
		/**
		*@brief{-1.0f,0.0f}
		*/
		static const Vector2 Left;
		/**
		*@brief{0.0f,1.0f}
		*/
		static const Vector2 Up;
		/**
		*@brief{0.0f,-1.0f}
		*/
		static const Vector2 Down;
		/**
		*@brief{0.0f,0.0f}
		*/
		static const Vector2 Zero;
		/**
		*@brief{1.0f,1.0f}
		*/
		static const Vector2 One;
		//======================
	};

	inline MVector MC_VECCALL  Vec2Set(const Vector2& v)
	{
		return _mm_set_ps(1.0f, 0.0f, v.y, v.x);
	}
	inline MVector MC_VECCALL  Vec2Set(float x, float y)
	{
		return _mm_set_ps(1.0f, 0.0f, y, x);
	}
	inline MVector MC_VECCALL  Vec2Set(float value)
	{
		return _mm_set_ps(1.0f, 0.0f, value, value);
	}
	inline void MC_VECCALL  MVecStore(Vector2* out, const MVector& in)
	{
		//( in.y, in.y, in.y, in.y).
		MVector y = _mm_shuffle_ps(in, in, _MM_SHUFFLE(1, 1, 1, 1));
		//in.xをxへ代入.
		_mm_store_ss(&out->x, in);
		//yを代入.
		_mm_store_ss(&out->y, y);
	}
	inline MVector MC_VECCALL  MLoadVec2(const Vector2* in)
	{
		MVector x = _mm_load_ss(&in->x);
		MVector y = _mm_load_ss(&in->y);
		return _mm_unpacklo_ps(x, y);
	}
	inline MVector MC_VECCALL  Vec2Add(MVector left, MVector right)
	{
		return _mm_add_ps(left, right);
	}
	inline MVector MC_VECCALL  Vec2Sub(MVector left, MVector right)
	{
		return _mm_sub_ps(left, right);
	}
	inline MVector MC_VECCALL  Vec2Mul(MVector left, MVector right)
	{
		return _mm_mul_ps(left, right);
	}
	inline MVector MC_VECCALL  Vec2Div(MVector left, MVector right)
	{
		return _mm_div_ps(left, right);
	}
	inline MVector MC_VECCALL Vec2Length(MVector v)
	{
		//vの各要素をvと乗算.
		MVector x2y2 = _mm_mul_ps(v, v);
		// ( x * x ) + (y * y ).
		MVector temp = _mm_add_ps(
			_mm_shuffle_ps(x2y2, x2y2, _MM_SHUFFLE(0, 0, 0, 0)),	//x2y2.x.
			_mm_shuffle_ps(x2y2, x2y2, _MM_SHUFFLE(1, 1, 1, 1)));	//x2y2.y.
		//tempの平方根.
		MVector lengthSq = _mm_sqrt_ps(temp);
		return lengthSq;
	}
	inline MVector MC_VECCALL Vec2Dot(MVector v)
	{
		//vの各要素をvと乗算.
		MVector x2y2 = _mm_mul_ps(v, v);
		//(x * aOther.x) + (y * aOther.y)
		return _mm_add_ps(
			_mm_shuffle_ps(x2y2, x2y2, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_shuffle_ps(x2y2, x2y2, _MM_SHUFFLE(1, 1, 1, 1)));
	}
	inline MVector MC_VECCALL Vec2Dot(MVector left, MVector right)
	{
		//leftの各要素をrightと乗算.
		MVector x2y2 = _mm_mul_ps(left, right);
		//(x * aOther.x) + (y * aOther.y)
		return _mm_add_ps(
			_mm_shuffle_ps(x2y2, x2y2, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_shuffle_ps(x2y2, x2y2, _MM_SHUFFLE(1, 1, 1, 1)));
	}
	inline MVector MC_VECCALL Vec2Cross(MVector left, MVector right)
	{
		MVector yx = _mm_shuffle_ps(right, right, _MM_SHUFFLE(0, 1, 0, 1));

		//(x * aRightOther.y) - (y * aRightOther.x).
		MVector dt = _mm_mul_ps(left, yx);
		return _mm_sub_ps(
			_mm_shuffle_ps(dt, dt, _MM_SHUFFLE(0, 0, 0, 0)),
			_mm_shuffle_ps(dt, dt, _MM_SHUFFLE(1, 1, 1, 1)));
	}
	inline MVector MC_VECCALL VecClamp(MVector own, MVector min, MVector max)
	{
		return _mm_min_ps(max, _mm_max_ps(min, own));
	}
	inline MVector MC_VECCALL VecLerp(MVector start, MVector end, MVector time)
	{
		return _mm_add_ps(_mm_mul_ps(_mm_sub_ps(end, start), time), start);
	}
	inline MVector MC_VECCALL Vec2TransformCoord(MVector v, MMATRIX m)
	{
		MVector Y = XM_PERMUTE_PS(v, _MM_SHUFFLE(1, 1, 1, 1));
		MVector X = XM_PERMUTE_PS(v, _MM_SHUFFLE(0, 0, 0, 0));

		MVector Result = Simd::VectorMultiplyAdd(Y, m.r[1], m.r[3]);
		Result = Simd::VectorMultiplyAdd(X, m.r[0], Result);

		MVector W = XM_PERMUTE_PS(v, _MM_SHUFFLE(3, 3, 3, 3));
		return _mm_div_ps(Result, W);
	}
	inline MVector MC_VECCALL Vec2TransformNormal(MVector v, MMATRIX m)
	{
		MVector vResult = XM_PERMUTE_PS(v, _MM_SHUFFLE(0, 0, 0, 0));
		vResult = _mm_mul_ps(vResult, m.r[0]);
		MVector vTemp = XM_PERMUTE_PS(v, _MM_SHUFFLE(1, 1, 1, 1));
		vTemp = _mm_mul_ps(vTemp, m.r[1]);
		vResult = _mm_add_ps(vResult, vTemp);
		return vResult;
	}
	inline MVector MC_VECCALL Vec2CatmullRom(MVector v1, MVector v2, MVector v3, MVector v4, float time)
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
	inline MVector MC_VECCALL Vec2Hermite(MVector posVec1, MVector tangentVec1, MVector posVec2, MVector tangentVec2, float time)
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