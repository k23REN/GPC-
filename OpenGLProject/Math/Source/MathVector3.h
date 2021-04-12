#pragma once

namespace MadCanyon
{
	class Quaternion;
	class Vector4;
	class Vector3 : public DirectX::XMFLOAT3
	{
	public:
		// �R���X�g���N�^��0����
		Vector3()
		{
			x = 0;
			y = 0;
			z = 0;
		}
		// ���W�w��t���R���X�g���N�^
		Vector3(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}

		// XMVECTOR���������Ă�����
		Vector3(const MVector& V)
		{
			MVecStore(this, V);
		}
		// XMFLOAT3���������Ă�����
		Vector3(const DirectX::XMFLOAT3& V)
		{
			x = V.x;
			y = V.y;
			z = V.z;
		}

		// XMVECTOR�֕ϊ�
		operator MVector() const{return MLoadVec3(this);}
		// Math::Vector3�ƌ݊����������߂̊֐�
		operator DirectX::SimpleMath::Vector3& () { return *(DirectX::SimpleMath::Vector3*)this; }

		// ���Z�q
		Vector3& operator+= (const Vector3& v)
		{
			*this = Vec3Add(Vec3Set(*this), Vec3Set(v));
			return *this;
		}
		Vector3& operator-= (const Vector3& v)
		{
			*this = Vec3Sub(Vec3Set(*this), Vec3Set(v));
			return *this;
		}
		Vector3& operator*= (const Vector3& v)
		{
			*this = Vec3Mul(Vec3Set(*this), Vec3Set(v));
			return *this;
		}
		Vector3& operator*= (float s)
		{
			*this = Vec3Mul(Vec3Set(*this), _mm_set_ps1(s));
			return *this;
		}
		Vector3& operator/= (float s)
		{
			assert(s != 0.0f);
			*this = Vec3Mul(Vec3Set(*this), _mm_set_ps1(1.f / s));
			return *this;
		}

		Vector3 operator+ (const Vector3& v)const{ return Vec3Add(Vec3Set(*this), Vec3Set(v)); }
		Vector3 operator- (const Vector3& v)const{ return Vec3Sub(Vec3Set(*this), Vec3Set(v)); }
		Vector3 operator* (const Vector3& v)const{ return Vec3Mul(Vec3Set(*this), Vec3Set(v)); }
		Vector3 operator/ (const Vector3& v)const{ return Vec3Div(Vec3Set(*this), Vec3Set(v)); }
		Vector3 operator* (float s)const { return Vec3Mul(Vec3Set(*this), Vec3Set(s)); }
		Vector3 operator/ (float s)const 
		{
			assert(s != 0.0f);
			return Vec3Mul(Vec3Set(*this), _mm_set_ps1(1.f / s));
		}

		// �Z�b�g
		Vector3& Set(float fx, float fy, float fz)
		{
			x = fx;
			y = fy;
			z = fz;
			return *this;
		}

		//�S�Ă̗v�f��0.0f���ǂ���.
		bool IsAllZero()const { return x == 0.0f && y == 0.0f && z == 0.0f; }

		// �w������ɁAangle����������
		Vector3& LookTo(const Vector3& dir, float angle);

		// ����
		float Length() const;
		// �����̂Q��(�����Ȃ̂Ŕ���p�Ɏg�p���邱�Ƃ�����)
		float LengthSquared() const;

		/**
		* @brief �x�N�g���Ԃ̋�����Ԃ��܂�
		* @note	�������ʂ𓾂���̂Ȃ�o�������DistanceSqr���g�p���Ă�������
		*/
		float Distance(const Vector3& aOther)const { return (*this - aOther).Length(); }
		/**
		* @brief �x�N�g���Ԃ̋����̓���Ԃ��܂�
		*/
		float DistanceSqr(const Vector3& aOther)const { return (*this - aOther).LengthSquared(); }

		// ����
		float Dot(const Vector3& v) const;
		// ����
		static float sDot(const Vector3& v1, const Vector3& v2);

		// �O��
		Vector3 Cross(const Vector3& v)const;
		// �O��
		static Vector3 sCross(const Vector3& v1, const Vector3& v2);

		/**
		* @brief �x�N�g���Ƃ̊p�x(���W�A���p)��Ԃ��܂�
		* @note	�f�O���[�p�ł���-180�x�`180�x�̊p�x���Ԃ�܂��B
		*		aOther��this���E�Ȃ�v���X�A���Ȃ�}�C�i�X�̊p�x�ɂȂ�܂��B
		*/
		float Angle(const Vector3& other)const;
		/**
		* @brief aOther��this���E���w���Ă����true��Ԃ��܂�
		*/
		bool IsRight(const Vector3& other)const { return this->Cross(other).y > 0.0f; }

		// �����𐳋K��
		Vector3& Normalize();
		Vector3 Normalized()const { return Vector3{ *this }.Normalize(); }

		// �N�����v
		Vector3& Clamp(const Vector3& minVec, const Vector3& maxVec);

		// ���`���
		Vector3& Lerp(const Vector3& startVec, const Vector3& endVec, float time);

		// �X���[�Y�X�e�b�v���
		Vector3& SmoothStep(const Vector3& startVec, const Vector3& endVec, float time);

		// Catmull-Rom�X�v���C�����
		Vector3& CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t);

		// �G���~�l�[�g�X�v���C�����
		Vector3& Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t);
		// �N�H�[�^�j�I���ŕϊ�
		Vector3& Transform(const Quaternion& quat);

		// �s��ŕϊ����Aw=1�ɓ��e����B
		Vector3& TransformCoord(const Matrix44& m);

		// (x,y,z,0)�ŕϊ�
		Vector3& TransformNormal(const Matrix44& m);

		// �z��
		Vector<float> ToArray() const
		{
			return { x, y, z };
		}
		// Vector4��
		Vector4 ToVec4(float w);

		// float�z��̐擪�A�h���X�Ƃ��ĕԂ�
		operator const float* () const
		{
			return &x;
		}
		operator float* ()
		{
			return &x;
		}

		//===============�ÓI�ϐ�
		/**
		* @brief {1.0f,0.0f,0.0f}
		*/
		static const Vector3 Right;
		/**
		* @brief {-1.0f,0.0f,0.0f}
		*/
		static const Vector3 Left;
		/**
		* @brief {0.0f,1.0f,0.0f}
		*/
		static const Vector3 Up;
		/**
		* @brief {0.0f,-1.0f,0.0f}
		*/
		static const Vector3 Down;
		/**
		* @brief {0.0f,0.0f,1.0f}
		*/
		static const Vector3 Forward;
		/**
		* @brief {0.0f,0.0f,-1.0f}
		*/
		static const Vector3 Back;
		/**
		* @brief {0.0f,0.0f,0.0f}
		*/
		static const Vector3 Zero;
		/**
		* @brief {1.0f,1.0f,1.0f}
		*/
		static const Vector3 One;
		/**
		* @brief {1.0f,1.0f,1.0f}
		*/
		static const Vector3 Infinity;
		/**
		* @brief {1.0f,1.0f,1.0f}
		*/
		static const Vector3 NegativeInfinity;
		/**
		* @brief {1.0f,1.0f,1.0f}
		*/
		static const Vector3 PositiveInfinity;
	};
	MVector MC_VECCALL QuatConjugate(MVector v);
	MVector MC_VECCALL  QuatMul(MVector left, MVector right);
	inline MVector MC_VECCALL  Vec3Set(const Vector3& v)
	{
		return _mm_set_ps(1.0f, v.z, v.y, v.x);
	}
	inline MVector MC_VECCALL  Vec3Set(float x, float y, float z)
	{
		return _mm_set_ps(1.0f, z, y, x);
	}
	inline MVector MC_VECCALL  Vec3Set(float value)
	{
		return _mm_set_ps(1.0f, value, value, value);
	}
	inline void MC_VECCALL  MVecStore(Vector3* out, const MVector& in)
	{
		//( xmVec.y, xmVec.y, xmVec.y, xmVec.y).
		MVector y = _mm_shuffle_ps(in, in, _MM_SHUFFLE(1, 1, 1, 1));
		MVector z = _mm_shuffle_ps(in, in, _MM_SHUFFLE(2, 2, 2, 2));
		//xmVec.x��x�֑��.
		_mm_store_ss(&out->x, in);
		//xmVec.x��y�֑��.
		_mm_store_ss(&out->y, y);
		_mm_store_ss(&out->z, z);
	}
	inline MVector MC_VECCALL  MLoadVec3(const Vector3* in)
	{
		MVector x = _mm_load_ss(&in->x);
		MVector y = _mm_load_ss(&in->y);
		MVector z = _mm_load_ss(&in->z);
		MVector xy = _mm_unpacklo_ps(x, y);
		return _mm_movelh_ps(xy, z);
	}
	inline MVector MC_VECCALL  Vec3Add(MVector left, MVector right)
	{
		return _mm_add_ps(left, right);
	}
	inline MVector MC_VECCALL  Vec3Sub(MVector left, MVector right)
	{
		return _mm_sub_ps(left, right);
	}
	inline MVector MC_VECCALL  Vec3Mul(MVector left, MVector right)
	{
		return _mm_mul_ps(left, right);
	}
	inline MVector MC_VECCALL  Vec3Div(MVector left, MVector right)
	{
		return _mm_div_ps(left, right);
	}
	inline MVector MC_VECCALL Vec3Length(MVector v)
	{
		//x��y��z�̃h�b�g�ς����߂�.
		//*this * *this.
		MVector lengthSq = _mm_mul_ps(v, v);

		//lengthSq����y��z�̒l���擾.
		//lengthSq{x,y,z,w} -> temp{z,y,z,y}.
		MVector temp = _mm_shuffle_ps(lengthSq, lengthSq, _MM_SHUFFLE(1, 2, 1, 2));
		//lengthSq��x(����32�r�b�g) + temp��z(����32�r�b�g),�c����32�r�b�g��lengthSq�̒l���R�s�[.
		//x + z.
		lengthSq = _mm_add_ss(lengthSq, temp);
		//temp����y�̒l���擾.
		//temp{y,y,y,y}.
		temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));
		//lengthSq��x(����32�r�b�g) + temp��y(����32�r�b�g),�c����32�r�b�g��lengthSq�̒l���R�s�[.
		//x + z + y.
		lengthSq = _mm_add_ss(lengthSq, temp);
		//LengthSq��x(����32�r�b�g)���擾.
		lengthSq = _mm_shuffle_ps(lengthSq, lengthSq, _MM_SHUFFLE(0, 0, 0, 0));
		//lengthSq�̕��������v�Z.
		lengthSq = _mm_sqrt_ps(lengthSq);

		return lengthSq;
	}
	inline MVector MC_VECCALL Vec3Dot(MVector v)
	{
		//*this * vec.
		MVector dot = _mm_mul_ps(v, v);
		// x = dot.vector4_f32[1], y = dot.vector4_f32[2]
		MVector temp = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(2, 1, 2, 1));
		//dot.m128_f32[0] = x + y.
		dot = _mm_add_ss(dot, temp);
		//x = dot.m128_f32[2].
		temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));
		//dot.m_128_f32[0] = (x + y) + z.
		dot = _mm_add_ss(dot, temp);
		//dot�̑S�v�f��x�������A�擪��dot.m128_f32[0]��Ԃ�.
		return	_mm_shuffle_ps(dot, dot, _MM_SHUFFLE(0, 0, 0, 0));
	}
	inline MVector MC_VECCALL Vec3Dot(MVector left, MVector right)
	{
		//*this * vec.
		MVector dot = _mm_mul_ps(left, right);
		// x = dot.vector4_f32[1], y = dot.vector4_f32[2]
		MVector temp = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(2, 1, 2, 1));
		//dot.m128_f32[0] = x + y.
		dot = _mm_add_ss(dot, temp);
		//x = dot.m128_f32[2].
		temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));
		//dot.m_128_f32[0] = (x + y) + z.
		dot = _mm_add_ss(dot, temp);
		//dot�̑S�v�f��x�������A�擪��dot.m128_f32[0]��Ԃ�.
		return	_mm_shuffle_ps(dot, dot, _MM_SHUFFLE(0, 0, 0, 0));
	}
	inline MVector MC_VECCALL Vec3Cross(MVector left, MVector right)
	{
		// [ V1.y*V2.z - V1.z*V2.y,
		//   V1.z*V2.x - V1.x*V2.z,
		//   V1.x*V2.y - V1.y*V2.x ]
		//�Ō��w��1����
		
		return _mm_and_ps(_mm_sub_ps(
			_mm_mul_ps(_mm_shuffle_ps(left, left, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(right, right, _MM_SHUFFLE(3, 1, 0, 2))),
			_mm_mul_ps(_mm_shuffle_ps(left, left, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(right, right, _MM_SHUFFLE(3, 0, 2, 1)))
		), Simd::MV_Mask3);
	}
	inline MVector MC_VECCALL Vec3Normalize(MVector v)
	{
		//x,y,z���ꂼ��̃h�b�g�ς����߂�.
		MVector lengthSq = _mm_mul_ps(v, v);
		// x = lengthSq.vector4_f32[1], y = lengthSq.vector4_f32[2]
		MVector temp = _mm_shuffle_ps(lengthSq, lengthSq, _MM_SHUFFLE(2, 1, 2, 1));
		//lengthSq.x + temp.x
		lengthSq = _mm_add_ss(lengthSq, temp);
		//temp = { temp.y, temp.y, temp.y, temp.y }
		temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));

		//lengthSq.x + temp.x
		lengthSq = _mm_add_ss(lengthSq, temp);
		//lengthSq = {lengthSq.x, lengthSq.x, lengthSq.x, lengthSq.x }
		lengthSq = _mm_shuffle_ps(lengthSq, lengthSq, _MM_SHUFFLE(0, 0, 0, 0));
		//������.
		MVector result = _mm_sqrt_ps(lengthSq);
		//4�̒P���x���������_�l��0�N���A�������̂�zeroMask�ɑ��.
		MVector zeroMask = _mm_setzero_ps();

		zeroMask = _mm_cmpneq_ps(zeroMask, result);

		lengthSq = _mm_cmpneq_ps(lengthSq, Simd::MV_Infinity);

		result = _mm_div_ps(v, result);

		result = _mm_and_ps(result, zeroMask);

		MVector temp1 = _mm_andnot_ps(lengthSq, Simd::MV_QNaN);
		MVector temp2 = _mm_and_ps(result, lengthSq);

		result = _mm_or_ps(temp1, temp2);
		return result;
	}

	inline MVector MC_VECCALL Vec3Rotate(MVector v, MVector quat)
	{
		MVector A = Simd::VectorSelect(Simd::MV_Select1110.v, v, Simd::MV_Select1110.v);
		MVector Q = QuatConjugate(quat);
		MVector Result = QuatMul(Q, A);
		return QuatMul(Result, quat);
	}
	inline MVector MC_VECCALL Vec3TransformCoord(MVector v, MMATRIX m)
	{
		MVector Z = XM_PERMUTE_PS(v, _MM_SHUFFLE(2, 2, 2, 2));
		MVector Y = XM_PERMUTE_PS(v, _MM_SHUFFLE(1, 1, 1, 1));
		MVector X = XM_PERMUTE_PS(v, _MM_SHUFFLE(0, 0, 0, 0));

		MVector Result = Simd::VectorMultiplyAdd(Z, m.r[2], m.r[3]);
		Result = Simd::VectorMultiplyAdd(Y, m.r[1], Result);
		Result = Simd::VectorMultiplyAdd(X, m.r[0], Result);

		MVector W = XM_PERMUTE_PS(Result, _MM_SHUFFLE(3, 3, 3, 3));
		return _mm_div_ps(Result, W);
	}
	inline MVector MC_VECCALL Vec3TransformNormal(MVector v, MMATRIX m)
	{
		MVector vResult = XM_PERMUTE_PS(v, _MM_SHUFFLE(0, 0, 0, 0));
		vResult = _mm_mul_ps(vResult, m.r[0]);
		MVector vTemp = XM_PERMUTE_PS(v, _MM_SHUFFLE(1, 1, 1, 1));
		vTemp = _mm_mul_ps(vTemp, m.r[1]);
		vResult = _mm_add_ps(vResult, vTemp);
		vTemp = XM_PERMUTE_PS(v, _MM_SHUFFLE(2, 2, 2, 2));
		vTemp = _mm_mul_ps(vTemp, m.r[2]);
		vResult = _mm_add_ps(vResult, vTemp);
		return vResult;
	}
	inline MVector MC_VECCALL Vec3CatmullRom(MVector v1, MVector v2, MVector v3, MVector v4, float time)
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
	inline MVector MC_VECCALL Vec3Hermite(MVector posVec1, MVector tangentVec1, MVector posVec2, MVector tangentVec2, float time)
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