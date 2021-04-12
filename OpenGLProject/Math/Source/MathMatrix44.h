#pragma once

namespace MadCanyon
{
	class Vector3;
	class Quaternion;
	class Matrix44 : public DirectX::XMFLOAT4X4
	{
	public:
		// デフォルトコンストラクタは単位行列
		Matrix44()
		{
			*this = DirectX::XMMatrixIdentity();
		}

		// XMMATRIXから代入してきた
		Matrix44(const DirectX::XMMATRIX& M)
		{
			DirectX::XMStoreFloat4x4(this, M);
		}
		Matrix44(const MMATRIX& M)
		{
			memcpy_s(this, sizeof(float) * 16, &M, sizeof(MMATRIX));
		}
		// XMFLOAT4X4から代入してきた
		Matrix44(const DirectX::XMFLOAT4X4& M)
		{
			memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4));
		}

		Matrix44(float a11, float a12, float a13, float a14,
			float a21, float a22, float a23, float a24,
			float a31, float a32, float a33, float a34,
			float a41, float a42, float a43, float a44);

		// XMMATRIXへ変換
		operator DirectX::XMMATRIX() const
		{
			DirectX::XMMATRIX matrix;
			matrix.r[0] = _mm_loadu_ps(&this->_11);
			matrix.r[1] = _mm_loadu_ps(&this->_21);
			matrix.r[2] = _mm_loadu_ps(&this->_31);
			matrix.r[3] = _mm_loadu_ps(&this->_41);
			return matrix;
		}
		operator MMATRIX() const
		{
			MMATRIX matrix;
			matrix.r[0] = _mm_loadu_ps(&this->_11);
			matrix.r[1] = _mm_loadu_ps(&this->_21);
			matrix.r[2] = _mm_loadu_ps(&this->_31);
			matrix.r[3] = _mm_loadu_ps(&this->_41);
			return matrix;
		}
		// Math::Matrixと互換性を持つための関数
		operator DirectX::SimpleMath::Matrix& () { return *(DirectX::SimpleMath::Matrix*)this; }

		// 代入演算子
		Matrix44& operator*= (const Matrix44& other);

		Matrix44 operator* (const Matrix44& other)const;

		//=================================
		//
		// 作成
		//
		//=================================

		Matrix44& SetIdentity();

		// 単位行列にする
		Matrix44& CreateIdentity();

		// 移動行列作成
		Matrix44& CreateTranslation(float x, float y, float z);
		// 移動行列作成
		Matrix44& CreateTranslation(const Vector3& pos);
		// X回転行列作成
		Matrix44& CreateRotationX(float angle);
		// Y回転行列作成
		Matrix44& CreateRotationY(float angle);
		// Z回転行列作成
		Matrix44& CreateRotationZ(float angle);
		// 指定軸回転行列作成
		Matrix44& CreateRotationAxis(const Vector3& axis, float angle);
		// クォータニオンから回転行列作成
		Matrix44& CreateFromQuaternion(const Quaternion& quat);

		// 拡大、回転、座標から、行列を作成する
		Matrix44& CreateFromSRT(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);

		// 拡縮行列作成
		Matrix44& CreateScaling(float x, float y, float z);
		Matrix44& CreateScaling(const Vector3& scale);

		// 透視影行列作成
		Matrix44& CreateProjection_PerspectiveFov(float fovAngleY, float aspectRatio, float nearZ, float farZ);

		// 正射影行列作成
		Matrix44& CreateProjection_Orthographic(float viewWidth, float viewHeight, float nearZ, float farZ);


		//=================================
		//
		// 操作
		//
		//=================================

		// 逆行列にする
		Matrix44& Inverse();

		// thisの逆行列を返します
		Matrix44 Inversed()const { return Matrix44{ *this }.Inverse(); }

		// 移動
		Matrix44& Move(float x, float y, float z)
		{
			_41 += x;
			_42 += y;
			_43 += z;
			return *this;
		}
		Matrix44& Move(const Vector3& v)
		{
			_41 += v.x;
			_42 += v.y;
			_43 += v.z;
			return *this;
		}

		// X回転
		Matrix44& RotateX(float angle);
		// Y回転
		Matrix44& RotateY(float angle);
		// Z回転
		Matrix44& RotateZ(float angle);
		// 指定軸回転
		Matrix44& RotateAxis(const Vector3& axis, float angle);

		// 拡縮
		Matrix44& Scale(float x, float y, float z);
		Matrix44& Scale(const Vector3& v);

		// 合成
		Matrix44& Multiply(const Matrix44& m)
		{
			*this *= m;
			return *this;
		}
		Matrix44& CreateLookTo(const Vector3& dir, const Vector3& up = Vector3::Zero)
		{
			const auto TmpZ = dir.Normalized();
			const auto X = Vector3::Up.Cross(TmpZ);
			const auto Y = up.IsAllZero() ? TmpZ.Cross(X) : up;//Y軸を固定するか否か.
			const auto Z = X.Cross(Y);

			_11 = X.x; _12 = X.y; _13 = X.z; _14 = 0.0f;
			_21 = Y.x; _22 = Y.y; _23 = Y.z; _24 = 0.0f;
			_31 = Z.x; _32 = Z.y; _33 = Z.z; _34 = 0.0f;
			_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;

			return *this;
		}
		// Z軸を指定方向に向ける
		Matrix44& LookTo(const Vector3& dir, const Vector3& up = Vector3::Zero)
		{
			const auto TmpZ = dir.Normalized();
			const auto X = Vector3::Up.Cross(TmpZ);
			const auto Y = up.IsAllZero() ? TmpZ.Cross(X) : up;//Y軸を固定するか否か.
			const auto Z = X.Cross(Y);

			float scaleX = GetXAxis().Length();
			float scaleY = GetYAxis().Length();
			float scaleZ = GetZAxis().Length();

			SetXAxis(X * scaleX);
			SetYAxis(Y * scaleY);
			SetZAxis(Z * scaleZ);

			return *this;
		}

		Matrix44& CreateLookAt(const Vector3& position, const Vector3& look, const Vector3& worldUp = Vector3::Up);
		Matrix44& LookAt(const Vector3& lookPosition, const Vector3& worldUp = Vector3::Up);

		Matrix44& SetRotation(const Matrix44& matrix);

		// 拡大を正規化
		Matrix44& NormalizeScale()
		{
			Vector3 v;
			v = GetXAxis();
			SetXAxis(v.Normalize());
			v = GetYAxis();
			SetXAxis(v.Normalize());
			v = GetZAxis();
			SetZAxis(v.Normalize());

			return *this;
		}


		//=================================
		//
		// プロパティ
		//
		//=================================

		// X軸取得
		const Vector3& GetXAxis() const { return *reinterpret_cast<const Vector3*>(&_11); }
		Vector3 Right() const
		{
			Vector3 v{ _11, _12, _13 };
			v.Normalize();
			return v;
		}

		// X軸セット
		void SetXAxis(const Vector3& v)
		{
			_11 = v.x;
			_12 = v.y;
			_13 = v.z;
		}
		// Y軸取得
		const Vector3& GetYAxis() const { return *reinterpret_cast<const Vector3*>(&_21); }
		Vector3 Up() const
		{
			Vector3 v{ _21, _22, _23 };
			v.Normalize();
			return v;
		}

		// Y軸セット
		void SetYAxis(const Vector3& v)
		{
			_21 = v.x;
			_22 = v.y;
			_23 = v.z;
		}
		// Z軸取得
		const Vector3& GetZAxis() const { return *reinterpret_cast<const Vector3*>(&_31); }
		Vector3 Forward() const
		{
			Vector3 v{ _31, _32, _33 };
			v.Normalize();
			return v;
		}
		// Z軸セット
		void SetZAxis(const Vector3& v)
		{
			_31 = v.x;
			_32 = v.y;
			_33 = v.z;
		}

		// 座標取得
		const Vector3& GetPosition() const { return *reinterpret_cast<const Vector3*>(&_41); }
		// 座標セット
		void Translation(const Vector3& v)
		{
			_41 = v.x;
			_42 = v.y;
			_43 = v.z;
		}

		Matrix44& SetPosition(const Vector3& pos)
		{
			_41 = pos.x;
			_42 = pos.y;
			_43 = pos.z;
			return *this;
		}

		//　thisからクォータニオン取得
		const Quaternion GetRotation()const;

		// スケール取得
		Vector3 GetScale() const
		{
			return {
				GetXAxis().Length(),
				GetYAxis().Length(),
				GetZAxis().Length()
			};
		}

		// スケースセット
		void SetScale(const Vector3& v)
		{
			NormalizeScale();
			SetXAxis(GetXAxis() * v.x);
			SetYAxis(GetYAxis() * v.y);
			SetZAxis(GetZAxis() * v.z);
		}

		// XYZの順番で合成したときの、回転角度を算出する
		Vector3 ComputeAngles() const
		{
			Matrix44 mat = *this;
			mat.NormalizeScale();

			Vector3 angles;
			angles.x = atan2f(mat.m[1][2], mat.m[2][2]);
			angles.y = atan2f(-mat.m[0][2], sqrtf(mat.m[1][2] * mat.m[1][2] + mat.m[2][2] * mat.m[2][2]));
			angles.z = atan2f(mat.m[0][1], mat.m[0][0]);
			return angles;
		}

		// 行列を、拡大、回転、座標へ分解する
		bool Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) const;

		//渡された3Dベクトルをこの行列で変換
		Vector3 TransformPoint(const Vector3& v)const;

		//渡された3Dベクトルをこの行列で法線に変換
		Vector3 TransformVector(const Vector3& v)const;

		Matrix44& Transpose();

		// 配列化
		Vector<float> ToArray() const
		{
			return { _11, _12, _13, _14,
						_21, _22, _23, _24,
						_31, _32, _33, _34,
						_41, _42, _43, _44
			};
		}

		// float配列の先頭アドレスとして返す
		operator const float* () const
		{
			return &_11;
		}
		operator float* ()
		{
			return &_11;
		}
		//=================静的変数===============
		/**
		* @brief 単位行列
		*/
		static const Matrix44 Identity;
		/**
		* @brief 全て0
		*/
		static const Matrix44 Zero;
		//=======================================

	};
	inline MMATRIX MC_VECCALL LoadMatrix(const Matrix44* src)
	{
		MMATRIX M;
		M.r[0] = _mm_loadu_ps(&src->_11);
		M.r[1] = _mm_loadu_ps(&src->_21);
		M.r[2] = _mm_loadu_ps(&src->_31);
		M.r[3] = _mm_loadu_ps(&src->_41);
		return M;
	}

	inline MMATRIX MC_VECCALL MatrixTranslation(MVector trans)
	{
		MMATRIX M;
		M.r[0] = Simd::MV_IdentityR0.v;
		M.r[1] = Simd::MV_IdentityR1.v;
		M.r[2] = Simd::MV_IdentityR2.v;
		M.r[3] = trans;
		return M;
	}
	inline MMATRIX MC_VECCALL MatrixTranslation(float transX, float transY, float transZ)
	{
		MMATRIX M;
		M.r[0] = Simd::MV_IdentityR0.v;
		M.r[1] = Simd::MV_IdentityR1.v;
		M.r[2] = Simd::MV_IdentityR2.v;
		M.r[3] = _mm_set_ps(1.0f,transZ,transY,transX);
		return M;
	}
	inline MMATRIX MC_VECCALL MatrixScaling(float scaleX, float scaleY, float scaleZ)
	{
		MMATRIX M;
		M.r[0] = _mm_set_ps(0, 0, 0, scaleX);
		M.r[1] = _mm_set_ps(0, 0, scaleY, 0);
		M.r[2] = _mm_set_ps(0, scaleZ, 0, 0);
		M.r[3] = Simd::MV_IdentityR3.v;
		return M;
	}
	inline MMATRIX MC_VECCALL MatrixMultiply(MMATRIX left, MMATRIX right)
	{
		MMATRIX result;

		MVector w = left.r[0];
		MVector x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
		MVector y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
		MVector z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
		w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));

		x = _mm_mul_ps(x, right.r[0]);
		y = _mm_mul_ps(y, right.r[1]);
		z = _mm_mul_ps(z, right.r[2]);
		w = _mm_mul_ps(w, right.r[3]);

		x = _mm_add_ps(x, z);
		y = _mm_add_ps(y, w);
		x = _mm_add_ps(x, y);
		result.r[0] = x;

		w = left.r[1];
		x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
		y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
		z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
		w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));

		x = _mm_mul_ps(x, right.r[0]);
		y = _mm_mul_ps(y, right.r[1]);
		z = _mm_mul_ps(z, right.r[2]);
		w = _mm_mul_ps(w, right.r[3]);

		x = _mm_add_ps(x, z);
		y = _mm_add_ps(y, w);
		x = _mm_add_ps(x, y);
		result.r[1] = x;

		w = left.r[2];
		x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
		y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
		z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
		w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));

		x = _mm_mul_ps(x, right.r[0]);
		y = _mm_mul_ps(y, right.r[1]);
		z = _mm_mul_ps(z, right.r[2]);
		w = _mm_mul_ps(w, right.r[3]);
		x = _mm_add_ps(x, z);
		y = _mm_add_ps(y, w);
		x = _mm_add_ps(x, y);
		result.r[2] = x;

		w = left.r[3];
		x = _mm_shuffle_ps(w, w, _MM_SHUFFLE(0, 0, 0, 0));
		y = _mm_shuffle_ps(w, w, _MM_SHUFFLE(1, 1, 1, 1));
		z = _mm_shuffle_ps(w, w, _MM_SHUFFLE(2, 2, 2, 2));
		w = _mm_shuffle_ps(w, w, _MM_SHUFFLE(3, 3, 3, 3));

		x = _mm_mul_ps(x, right.r[0]);
		y = _mm_mul_ps(y, right.r[1]);
		z = _mm_mul_ps(z, right.r[2]);
		w = _mm_mul_ps(w, right.r[3]);
		x = _mm_add_ps(x, z);
		y = _mm_add_ps(y, w);
		x = _mm_add_ps(x, y);
		result.r[3] = x;

		return result;

	}
	inline MMATRIX MC_VECCALL MatrixRotationX(float angle)
	{
		float    SinAngle;
		float    CosAngle;
		Simd::ScalarSinCos(&SinAngle, &CosAngle, angle);

		MVector vSin = _mm_set_ss(SinAngle);
		MVector vCos = _mm_set_ss(CosAngle);
		// x = 0,y = cos,z = sin, w = 0
		vCos = _mm_shuffle_ps(vCos, vSin, _MM_SHUFFLE(3, 0, 0, 3));
		MMATRIX M;
		M.r[0] = Simd::MV_IdentityR0;
		M.r[1] = vCos;
		// x = 0,y = sin,z = cos, w = 0
		vCos = XM_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 1, 2, 0));
		// x = 0,y = -sin,z = cos, w = 0
		vCos = _mm_mul_ps(vCos, Simd::MV_NegateY);
		M.r[2] = vCos;
		M.r[3] = Simd::MV_IdentityR3;
		return M;
	}
	inline MMATRIX MC_VECCALL MatrixRotationY(float angle)
	{
		float    SinAngle;
		float    CosAngle;
		Simd::ScalarSinCos(&SinAngle, &CosAngle, angle);

		MVector vSin = _mm_set_ss(SinAngle);
		MVector vCos = _mm_set_ss(CosAngle);
		// x = sin,y = 0,z = cos, w = 0
		vSin = _mm_shuffle_ps(vSin, vCos, _MM_SHUFFLE(3, 0, 3, 0));
		MMATRIX M;
		M.r[2] = vSin;
		M.r[1] = Simd::MV_IdentityR1;
		// x = cos,y = 0,z = sin, w = 0
		vSin = XM_PERMUTE_PS(vSin, _MM_SHUFFLE(3, 0, 1, 2));
		// x = cos,y = 0,z = -sin, w = 0
		vSin = _mm_mul_ps(vSin, Simd::MV_NegateZ);
		M.r[0] = vSin;
		M.r[3] = Simd::MV_IdentityR3;
		return M;
	}
	inline MMATRIX MC_VECCALL MatrixRotationZ(float angle)
	{
		float    SinAngle;
		float    CosAngle;
		Simd::ScalarSinCos(&SinAngle, &CosAngle, angle);

		MVector vSin = _mm_set_ss(SinAngle);
		MVector vCos = _mm_set_ss(CosAngle);
		// x = cos,y = sin,z = 0, w = 0
		vCos = _mm_unpacklo_ps(vCos, vSin);
		MMATRIX M;
		M.r[0] = vCos;
		// x = sin,y = cos,z = 0, w = 0
		vCos = XM_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 2, 0, 1));
		// x = cos,y = -sin,z = 0, w = 0
		vCos = _mm_mul_ps(vCos, Simd::MV_NegateX);
		M.r[1] = vCos;
		M.r[2] = Simd::MV_IdentityR2;
		M.r[3] = Simd::MV_IdentityR3;
		return M;
	}
	inline MMATRIX MC_VECCALL MatrixRotationNormal(MVector normalAxis, float angle)
	{
		float    fSinAngle;
		float    fCosAngle;
		Simd::ScalarSinCos(&fSinAngle, &fCosAngle, angle);

		MVector C2 = _mm_set_ps1(1.0f - fCosAngle);
		MVector C1 = _mm_set_ps1(fCosAngle);
		MVector C0 = _mm_set_ps1(fSinAngle);

		MVector N0 = XM_PERMUTE_PS(normalAxis, _MM_SHUFFLE(3, 0, 2, 1));
		MVector N1 = XM_PERMUTE_PS(normalAxis, _MM_SHUFFLE(3, 1, 0, 2));

		MVector V0 = _mm_mul_ps(C2, N0);
		V0 = _mm_mul_ps(V0, N1);

		MVector R0 = _mm_mul_ps(C2, normalAxis);
		R0 = _mm_mul_ps(R0, normalAxis);
		R0 = _mm_add_ps(R0, C1);

		MVector R1 = _mm_mul_ps(C0, normalAxis);
		R1 = _mm_add_ps(R1, V0);
		MVector R2 = _mm_mul_ps(C0, normalAxis);
		R2 = _mm_sub_ps(V0, R2);

		V0 = _mm_and_ps(R0, Simd::MV_Mask3);
		MVector V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 1, 2, 0));
		V1 = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 3, 2, 1));
		MVector V2 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0, 0, 1, 1));
		V2 = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 0, 2, 0));

		R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1, 0, 3, 0));
		R2 = XM_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 2, 0));

		MMATRIX M;
		M.r[0] = R2;

		R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3, 2, 3, 1));
		R2 = XM_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 0, 2));
		M.r[1] = R2;

		V2 = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3, 2, 1, 0));
		M.r[2] = V2;
		M.r[3] = Simd::MV_IdentityR3.v;
		return M;
	}
	inline MMATRIX MC_VECCALL MatrixRotationAxis(MVector normalAxis, float angle)
	{
		return MatrixRotationNormal(normalAxis, angle);
	}
	inline MMATRIX MC_VECCALL MatrixScalingFromVector(MVector scale)
	{
		MMATRIX M;
		M.r[0] = _mm_and_ps(scale, Simd::MV_MaskX);
		M.r[1] = _mm_and_ps(scale, Simd::MV_MaskY);
		M.r[2] = _mm_and_ps(scale, Simd::MV_MaskZ);
		M.r[3] = Simd::MV_IdentityR3.v;
		return M;

	}
	inline MMATRIX MC_VECCALL MatrixRotationQuaternion(MVector quat)
	{
		//static const XMVECTORF32  Constant1110 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };
		static const MVECTORF32  Constant1110 = { 1.0f, 1.0f, 1.0f, 0.0f };

		MVector Q0 = _mm_add_ps(quat, quat);
		MVector Q1 = _mm_mul_ps(quat, Q0);

		MVector V0 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 0, 0, 1));
		V0 = _mm_and_ps(V0, Simd::MV_Mask3);
		MVector V1 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 1, 2, 2));
		V1 = _mm_and_ps(V1, Simd::MV_Mask3);
		MVector R0 = _mm_sub_ps(Constant1110, V0);
		R0 = _mm_sub_ps(R0, V1);

		V0 = XM_PERMUTE_PS(quat, _MM_SHUFFLE(3, 1, 0, 0));
		V1 = XM_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 2, 1, 2));
		V0 = _mm_mul_ps(V0, V1);

		V1 = XM_PERMUTE_PS(quat, _MM_SHUFFLE(3, 3, 3, 3));
		MVector V2 = XM_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 0, 2, 1));
		V1 = _mm_mul_ps(V1, V2);

		MVector R1 = _mm_add_ps(V0, V1);
		MVector R2 = _mm_sub_ps(V0, V1);

		V0 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(1, 0, 2, 1));
		V0 = XM_PERMUTE_PS(V0, _MM_SHUFFLE(1, 3, 2, 0));
		V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 2, 0, 0));
		V1 = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 0, 2, 0));

		Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(1, 0, 3, 0));
		Q1 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 2, 0));

		MMATRIX M;
		M.r[0] = Q1;

		Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(3, 2, 3, 1));
		Q1 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 0, 2));
		M.r[1] = Q1;

		Q1 = _mm_shuffle_ps(V1, R0, _MM_SHUFFLE(3, 2, 1, 0));
		M.r[2] = Q1;
		M.r[3] = Simd::MV_IdentityR3;
		return M;

	}
	inline MMATRIX MC_VECCALL MatrixTranslationFromVector(MVector v)
	{
		MMATRIX M;
		M.r[0] = Simd::MV_IdentityR0.v;
		M.r[1] = Simd::MV_IdentityR1.v;
		M.r[2] = Simd::MV_IdentityR2.v;
		M.r[3] = Simd::VectorSelect(Simd::MV_IdentityR3.v, v, Simd::MV_Select1110.v);
		return M;
	}
	inline MMATRIX MC_VECCALL MMatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
	{
		float    SinFov;
		float    CosFov;
		Simd::ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

		float fRange = FarZ / (FarZ - NearZ);
		// Note: This is recorded on the stack
		float Height = CosFov / SinFov;
		MVector rMem = {
			Height / AspectRatio,
			Height,
			fRange,
			-fRange * NearZ
		};
		// Copy from memory to SSE register
		MVector vValues = rMem;
		MVector vTemp = _mm_setzero_ps();
		// Copy x only
		vTemp = _mm_move_ss(vTemp, vValues);
		// CosFov / SinFov,0,0,0
		MMATRIX M;
		M.r[0] = vTemp;
		// 0,Height / AspectRatio,0,0
		vTemp = vValues;
		vTemp = _mm_and_ps(vTemp, Simd::MV_MaskY);
		M.r[1] = vTemp;
		// x=fRange,y=-fRange * NearZ,0,1.0f
		vTemp = _mm_setzero_ps();
		vValues = _mm_shuffle_ps(vValues, Simd::MV_IdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
		// 0,0,fRange,1.0f
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
		M.r[2] = vTemp;
		// 0,0,-fRange * NearZ,0.0f
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
		M.r[3] = vTemp;
		return M;
	}
	inline MMATRIX MC_VECCALL MatrixOrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
	{
		MMATRIX M;
		float fRange = 1.0f / (FarZ - NearZ);
		// Note: This is recorded on the stack
		MVector rMem = {
			2.0f / ViewWidth,
			2.0f / ViewHeight,
			fRange,
			-fRange * NearZ
		};
		// Copy from memory to SSE register
		MVector vValues = rMem;
		MVector vTemp = _mm_setzero_ps();
		// Copy x only
		vTemp = _mm_move_ss(vTemp, vValues);
		// 2.0f / ViewWidth,0,0,0
		M.r[0] = vTemp;
		// 0,2.0f / ViewHeight,0,0
		vTemp = vValues;
		vTemp = _mm_and_ps(vTemp, Simd::MV_MaskY);
		M.r[1] = vTemp;
		// x=fRange,y=-fRange * NearZ,0,1.0f
		vTemp = _mm_setzero_ps();
		vValues = _mm_shuffle_ps(vValues, Simd::MV_IdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
		// 0,0,fRange,0.0f
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
		M.r[2] = vTemp;
		// 0,0,-fRange * NearZ,1.0f
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
		M.r[3] = vTemp;
		return M;
	}
	inline MMATRIX MC_VECCALL MMatrixTranspose(MMATRIX& M)
	{
		// x.x,x.y,y.x,y.y
		MVector vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
		// x.z,x.w,y.z,y.w
		MVector vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
		// z.x,z.y,w.x,w.y
		MVector vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
		// z.z,z.w,w.z,w.w
		MVector vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));

		MMATRIX mResult;
		// x.x,y.x,z.x,w.x
		mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
		// x.y,y.y,z.y,w.y
		mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
		// x.z,y.z,z.z,w.z
		mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
		// x.w,y.w,z.w,w.w
		mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
		return mResult;

	}
	inline MMATRIX MC_VECCALL MatrixInverse(MVector* pDeterminant, MMATRIX M)
	{
		MMATRIX MT = MMatrixTranspose(M);
		MVector V00 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 1, 0, 0));
		MVector V10 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(3, 2, 3, 2));
		MVector V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 1, 0, 0));
		MVector V11 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(3, 2, 3, 2));
		MVector V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(2, 0, 2, 0));
		MVector V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(3, 1, 3, 1));

		MVector D0 = _mm_mul_ps(V00, V10);
		MVector D1 = _mm_mul_ps(V01, V11);
		MVector D2 = _mm_mul_ps(V02, V12);

		V00 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(3, 2, 3, 2));
		V10 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 1, 0, 0));
		V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(3, 2, 3, 2));
		V11 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 1, 0, 0));
		V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(3, 1, 3, 1));
		V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(2, 0, 2, 0));

		V00 = _mm_mul_ps(V00, V10);
		V01 = _mm_mul_ps(V01, V11);
		V02 = _mm_mul_ps(V02, V12);
		D0 = _mm_sub_ps(D0, V00);
		D1 = _mm_sub_ps(D1, V01);
		D2 = _mm_sub_ps(D2, V02);
		// V11 = D0Y,D0W,D2Y,D2Y
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
		V00 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 0, 2, 1));
		V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
		V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(0, 1, 0, 2));
		V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
		// V13 = D1Y,D1W,D2W,D2W
		MVector V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
		V02 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 0, 2, 1));
		V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
		MVector V03 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(0, 1, 0, 2));
		V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

		MVector C0 = _mm_mul_ps(V00, V10);
		MVector C2 = _mm_mul_ps(V01, V11);
		MVector C4 = _mm_mul_ps(V02, V12);
		MVector C6 = _mm_mul_ps(V03, V13);

		// V11 = D0X,D0Y,D2X,D2X
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
		V00 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(2, 1, 3, 2));
		V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
		V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 3, 2, 3));
		V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
		// V13 = D1X,D1Y,D2Z,D2Z
		V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
		V02 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(2, 1, 3, 2));
		V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
		V03 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 3, 2, 3));
		V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

		V00 = _mm_mul_ps(V00, V10);
		V01 = _mm_mul_ps(V01, V11);
		V02 = _mm_mul_ps(V02, V12);
		V03 = _mm_mul_ps(V03, V13);
		C0 = _mm_sub_ps(C0, V00);
		C2 = _mm_sub_ps(C2, V01);
		C4 = _mm_sub_ps(C4, V02);
		C6 = _mm_sub_ps(C6, V03);

		V00 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(0, 3, 0, 3));
		// V10 = D0Z,D0Z,D2X,D2Y
		V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
		V10 = XM_PERMUTE_PS(V10, _MM_SHUFFLE(0, 2, 3, 0));
		V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(2, 0, 3, 1));
		// V11 = D0X,D0W,D2X,D2Y
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
		V11 = XM_PERMUTE_PS(V11, _MM_SHUFFLE(2, 1, 0, 3));
		V02 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(0, 3, 0, 3));
		// V12 = D1Z,D1Z,D2Z,D2W
		V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
		V12 = XM_PERMUTE_PS(V12, _MM_SHUFFLE(0, 2, 3, 0));
		V03 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(2, 0, 3, 1));
		// V13 = D1X,D1W,D2Z,D2W
		V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
		V13 = XM_PERMUTE_PS(V13, _MM_SHUFFLE(2, 1, 0, 3));

		V00 = _mm_mul_ps(V00, V10);
		V01 = _mm_mul_ps(V01, V11);
		V02 = _mm_mul_ps(V02, V12);
		V03 = _mm_mul_ps(V03, V13);
		MVector C1 = _mm_sub_ps(C0, V00);
		C0 = _mm_add_ps(C0, V00);
		MVector C3 = _mm_add_ps(C2, V01);
		C2 = _mm_sub_ps(C2, V01);
		MVector C5 = _mm_sub_ps(C4, V02);
		C4 = _mm_add_ps(C4, V02);
		MVector C7 = _mm_add_ps(C6, V03);
		C6 = _mm_sub_ps(C6, V03);

		C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
		C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
		C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
		C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
		C0 = XM_PERMUTE_PS(C0, _MM_SHUFFLE(3, 1, 2, 0));
		C2 = XM_PERMUTE_PS(C2, _MM_SHUFFLE(3, 1, 2, 0));
		C4 = XM_PERMUTE_PS(C4, _MM_SHUFFLE(3, 1, 2, 0));
		C6 = XM_PERMUTE_PS(C6, _MM_SHUFFLE(3, 1, 2, 0));
		// Get the determinate
		MVector vTemp = Vec4Dot(C0, MT.r[0]);
		if (pDeterminant != nullptr)
			*pDeterminant = vTemp;
		vTemp = _mm_div_ps(Simd::MV_One, vTemp);
		MMATRIX mResult;
		mResult.r[0] = _mm_mul_ps(C0, vTemp);
		mResult.r[1] = _mm_mul_ps(C2, vTemp);
		mResult.r[2] = _mm_mul_ps(C4, vTemp);
		mResult.r[3] = _mm_mul_ps(C6, vTemp);
		return mResult;
	}
	inline MVector MC_VECCALL MatrixDeterminant(MMATRIX& M)
	{
		//static const XMVECTORF32 Sign = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };
		static const MVector Sign = { 1.0f, -1.0f, 1.0f, -1.0f };
		using namespace Simd;
		MVector V0 = MVectorSwizzle<MC_SWIZZLE_Y, MC_SWIZZLE_X, MC_SWIZZLE_X, MC_SWIZZLE_X>(M.r[2]);
		MVector V1 = MVectorSwizzle<MC_SWIZZLE_Z, MC_SWIZZLE_Z, MC_SWIZZLE_Y, MC_SWIZZLE_Y>(M.r[3]);
		MVector V2 = MVectorSwizzle<MC_SWIZZLE_Y, MC_SWIZZLE_X, MC_SWIZZLE_X, MC_SWIZZLE_X>(M.r[2]);
		MVector V3 = MVectorSwizzle<MC_SWIZZLE_W, MC_SWIZZLE_W, MC_SWIZZLE_W, MC_SWIZZLE_Z>(M.r[3]);
		MVector V4 = MVectorSwizzle<MC_SWIZZLE_Z, MC_SWIZZLE_Z, MC_SWIZZLE_Y, MC_SWIZZLE_Y>(M.r[2]);
		MVector V5 = MVectorSwizzle<MC_SWIZZLE_W, MC_SWIZZLE_W, MC_SWIZZLE_W, MC_SWIZZLE_Z>(M.r[3]);

		MVector P0 = Vec4Mul(V0, V1);
		MVector P1 = Vec4Mul(V2, V3);
		MVector P2 = Vec4Mul(V4, V5);

		V0 = MVectorSwizzle<MC_SWIZZLE_Z, MC_SWIZZLE_Z, MC_SWIZZLE_Y, MC_SWIZZLE_Y>(M.r[2]);
		V1 = MVectorSwizzle<MC_SWIZZLE_Y, MC_SWIZZLE_X, MC_SWIZZLE_X, MC_SWIZZLE_X>(M.r[3]);
		V2 = MVectorSwizzle<MC_SWIZZLE_W, MC_SWIZZLE_W, MC_SWIZZLE_W, MC_SWIZZLE_Z>(M.r[2]);
		V3 = MVectorSwizzle<MC_SWIZZLE_Y, MC_SWIZZLE_X, MC_SWIZZLE_X, MC_SWIZZLE_X>(M.r[3]);
		V4 = MVectorSwizzle<MC_SWIZZLE_W, MC_SWIZZLE_W, MC_SWIZZLE_W, MC_SWIZZLE_Z>(M.r[2]);
		V5 = MVectorSwizzle<MC_SWIZZLE_Z, MC_SWIZZLE_Z, MC_SWIZZLE_Y, MC_SWIZZLE_Y>(M.r[3]);

		P0 = Simd::VectorNegativeMultiplySubtract(V0, V1, P0);
		P1 = Simd::VectorNegativeMultiplySubtract(V2, V3, P1);
		P2 = Simd::VectorNegativeMultiplySubtract(V4, V5, P2);

		V0 = MVectorSwizzle<MC_SWIZZLE_W, MC_SWIZZLE_W, MC_SWIZZLE_W, MC_SWIZZLE_Z>(M.r[1]);
		V1 = MVectorSwizzle<MC_SWIZZLE_Z, MC_SWIZZLE_Z, MC_SWIZZLE_Y, MC_SWIZZLE_Y>(M.r[1]);
		V2 = MVectorSwizzle<MC_SWIZZLE_Y, MC_SWIZZLE_X, MC_SWIZZLE_X, MC_SWIZZLE_X>(M.r[1]);

		MVector S = Vec4Mul(M.r[0], Sign);
		MVector R = Vec4Mul(V0, P0);
		R = Simd::VectorNegativeMultiplySubtract(V1, P1, R);
		R = Simd::VectorMultiplyAdd(V2, P2, R);

		return Vec4Dot(S, R);
	}
	inline bool MC_VECCALL MatrixDecompose(MVector* outScale, MVector* outRotQuat, MVector* outTrans, MMATRIX M)
	{
		static const MVector* pvCanonicalBasis[3] = {
			&Simd::MV_IdentityR0.v,
			&Simd::MV_IdentityR1.v,
			&Simd::MV_IdentityR2.v
		};

		assert(outScale != nullptr);
		assert(outRotQuat != nullptr);
		assert(outTrans != nullptr);

		//trans取得
		outTrans[0] = M.r[3];

		MVector* ppvBasis[3];
		MMATRIX matTemp;
		ppvBasis[0] = &matTemp.r[0];
		ppvBasis[1] = &matTemp.r[1];
		ppvBasis[2] = &matTemp.r[2];

		matTemp.r[0] = M.r[0];
		matTemp.r[1] = M.r[1];
		matTemp.r[2] = M.r[2];
		matTemp.r[3] = Simd::MV_IdentityR3.v;

		auto pfScales = reinterpret_cast<float*>(outScale);

		size_t a, b, c;
		Simd::VectorGetXPtr(&pfScales[0], Vec3Length(ppvBasis[0][0]));
		Simd::VectorGetXPtr(&pfScales[1], Vec3Length(ppvBasis[1][0]));
		Simd::VectorGetXPtr(&pfScales[2], Vec3Length(ppvBasis[2][0]));
		pfScales[3] = 0.f;

		M3RANKDECOMPOSE(a, b, c, pfScales[0], pfScales[1], pfScales[2])

			if (pfScales[a] < M3_DECOMP_EPSILON)
			{
				ppvBasis[a][0] = pvCanonicalBasis[a][0];
			}
		ppvBasis[a][0] =Vec3Normalize(ppvBasis[a][0]);

		if (pfScales[b] < M3_DECOMP_EPSILON)
		{
			size_t aa, bb, cc;
			float fAbsX, fAbsY, fAbsZ;

			fAbsX = fabsf(Simd::VectorGetX(ppvBasis[a][0]));
			fAbsY = fabsf(Simd::VectorGetY(ppvBasis[a][0]));
			fAbsZ = fabsf(Simd::VectorGetZ(ppvBasis[a][0]));

			M3RANKDECOMPOSE(aa, bb, cc, fAbsX, fAbsY, fAbsZ)

				ppvBasis[b][0] = Vec3Cross(ppvBasis[a][0], pvCanonicalBasis[cc][0]);
		}

		ppvBasis[b][0] = Vec3Normalize(ppvBasis[b][0]);

		if (pfScales[c] < M3_DECOMP_EPSILON)
		{
			ppvBasis[c][0] = Vec3Cross(ppvBasis[a][0], ppvBasis[b][0]);
		}

		ppvBasis[c][0] = Vec3Normalize(ppvBasis[c][0]);

		float fDet = Simd::VectorGetX(MatrixDeterminant(matTemp));

		// use Kramer's rule to check for handedness of coordinate system
		if (fDet < 0.0f)
		{
			// switch coordinate system by negating the scale and inverting the basis vector on the x-axis
			pfScales[a] = -pfScales[a];
			ppvBasis[a][0] = Simd::VectorNegate(ppvBasis[a][0]);

			fDet = -fDet;
		}

		fDet -= 1.0f;
		fDet *= fDet;

		if (M3_DECOMP_EPSILON < fDet)
		{
			// Non-SRT matrix encountered
			return false;
		}

		// generate the quaternion from the matrix
		outRotQuat[0] = QuaternionRotationMatrix(matTemp);
		return true;

	}
	inline MVector MC_VECCALL Vector3Transform(MVector v, MMATRIX m)
	{
		MVector vResult = XM_PERMUTE_PS(v, _MM_SHUFFLE(0, 0, 0, 0));
		vResult = _mm_mul_ps(vResult, m.r[0]);
		MVector vTemp = XM_PERMUTE_PS(v, _MM_SHUFFLE(1, 1, 1, 1));
		vTemp = _mm_mul_ps(vTemp, m.r[1]);
		vResult = _mm_add_ps(vResult, vTemp);
		vTemp = XM_PERMUTE_PS(v, _MM_SHUFFLE(2, 2, 2, 2));
		vTemp = _mm_mul_ps(vTemp, m.r[2]);
		vResult = _mm_add_ps(vResult, vTemp);
		vResult = _mm_add_ps(vResult, m.r[3]);
		return vResult;
	}
	inline MVector MC_VECCALL Vector3TransformNormal(MVector v, MMATRIX m)
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
	inline MMATRIX MC_VECCALL MatrixTranspose(MMATRIX M)
	{
		// x.x,x.y,y.x,y.y
		MVector vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
		// x.z,x.w,y.z,y.w
		MVector vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
		// z.x,z.y,w.x,w.y
		MVector vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
		// z.z,z.w,w.z,w.w
		MVector vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));
		MMATRIX mResult;

		// x.x,y.x,z.x,w.x
		mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
		// x.y,y.y,z.y,w.y
		mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
		// x.z,y.z,z.z,w.z
		mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
		// x.w,y.w,z.w,w.w
		mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
		return mResult;
	}
	inline MMATRIX MC_VECCALL MatrixLookToLH(MVector eyePos, MVector eyeDir, MVector upDir)
	{
		assert(!DirectX::XMVector3Equal(eyeDir, _mm_setzero_ps()));
		assert(!DirectX::XMVector3IsInfinite(eyeDir));
		assert(!DirectX::XMVector3Equal(upDir, _mm_setzero_ps()));
		assert(!DirectX::XMVector3IsInfinite(upDir));

		MVector R2 = Vec3Normalize(eyeDir);

		MVector R0 = Vec3Cross(upDir, R2);
		R0 = Vec3Normalize(R0);

		MVector R1 = Vec3Cross(R2, R0);

		MVector NegEyePosition = Simd::VectorNegate(eyePos);

		MVector D0 = Vec3Dot(R0, NegEyePosition);
		MVector D1 = Vec3Dot(R1, NegEyePosition);
		MVector D2 = Vec3Dot(R2, NegEyePosition);

		MMATRIX M;
		M.r[0] = Simd::VectorSelect(D0, R0, Simd::MV_Select1110.v);
		M.r[1] = Simd::VectorSelect(D1, R1, Simd::MV_Select1110.v);
		M.r[2] = Simd::VectorSelect(D2, R2, Simd::MV_Select1110.v);
		M.r[3] = Simd::MV_IdentityR3.v;

		M = MatrixTranspose(M);

		return M;
	}
}