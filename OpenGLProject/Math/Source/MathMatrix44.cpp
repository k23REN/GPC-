#include "MathMatrix44.h"
#include "MathIncludes.h"

namespace MadCanyon
{
	const Matrix44 Matrix44::Identity{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	const Matrix44 Matrix44::Zero{
			0.0f,0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,0.0f
	};

	Matrix44::Matrix44(float a11, float a12, float a13, float a14,
		float a21, float a22, float a23, float a24,
		float a31, float a32, float a33, float a34,
		float a41, float a42, float a43, float a44)
	{
		m[0][0] = a11; m[0][1] = a12; m[0][2] = a13; m[0][3] = a14;
		m[1][0] = a21; m[1][1] = a22; m[1][2] = a23; m[1][3] = a24;
		m[2][0] = a31; m[2][1] = a32; m[2][2] = a33; m[2][3] = a34;
		m[3][0] = a41; m[3][1] = a42; m[3][2] = a43; m[3][3] = a44;
	}

	Matrix44& Matrix44::CreateIdentity()
	{
		*this = Matrix44::Identity;
		return *this;
	}

	Matrix44& Matrix44::CreateLookAt(const Vector3& position, const Vector3& look, const Vector3& worldUp)
	{
		Vector3 tmp = Vec3Sub(Vec3Set(look), Vec3Set(position));
		const auto tmpZ = tmp.Normalize();
		const auto X = Vector3::Up.Cross(tmpZ);
		const auto Y = worldUp.IsAllZero() ? tmpZ.Cross(X) : worldUp;//Y²‚ğŒÅ’è‚·‚é‚©”Û‚©.
		const auto Z = X.Cross(Y);

		_11 = X.x; _12 = X.y; _13 = X.z; _14 = 0.0f;
		_21 = Y.x; _22 = Y.y; _23 = Y.z; _24 = 0.0f;
		_31 = Z.x; _32 = Z.y; _33 = Z.z; _34 = 0.0f;
		_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;

		return *this;
	}

	Matrix44& Matrix44::operator*=(const Matrix44& other)
	{
		*this = MatrixMultiply(*this, other);
		return *this;
	}

	Matrix44 Matrix44::operator*(const Matrix44& other) const
	{
		return MatrixMultiply(*this, other);
	}

	Matrix44& Matrix44::SetIdentity()
	{
		return *this = Matrix44::Identity;
	}

	Matrix44& Matrix44::CreateTranslation(float x, float y, float z)
	{
		*this = MatrixTranslation(Vec4Set(x, y, z, 1.0f));
		return *this;
	}

	Matrix44& Matrix44::CreateTranslation(const Vector3& pos)
	{
		*this = MatrixTranslation(Vec3Set(pos));
		return *this;
	}

	Matrix44& Matrix44::CreateRotationX(float angleRadian)
	{
		*this = MatrixRotationX(angleRadian);
		return *this;
	}

	Matrix44& Matrix44::CreateRotationY(float angleRadian)
	{
		*this = MatrixRotationY(angleRadian);
		return *this;
	}

	Matrix44& Matrix44::CreateRotationZ(float angleRadian)
	{
		*this = MatrixRotationZ(angleRadian);
		return *this;
	}

	Matrix44& Matrix44::CreateRotationAxis(const Vector3& axisNorm, float angleRadian)
	{
		if (axisNorm.IsAllZero()) { return *this; }
		*this = MatrixRotationAxis(Vec3Set(axisNorm.Normalized()), angleRadian);
		return *this;
	}

	Matrix44& Matrix44::CreateFromQuaternion(const Quaternion& quat)
	{
		*this = MatrixRotationQuaternion(quat);
		return *this;
	}

	Matrix44& Matrix44::CreateFromSRT(const Vector3& scale, const Quaternion& rotation, const Vector3& translation)
	{
		*this = MatrixScalingFromVector(scale) * MatrixRotationQuaternion(rotation) * MatrixTranslationFromVector(translation);
		return *this;
	}

	Matrix44& Matrix44::CreateScaling(const Vector3& scale)
	{
		*this = MatrixScaling(scale.x, scale.y, scale.z);
		return *this;
	}

	Matrix44& Matrix44::CreateScaling(float scaleX, float scaleY, float scaleZ)
	{
		*this = MatrixScaling(scaleX, scaleY, scaleZ);
		return *this;
	}

	Matrix44& Matrix44::CreateProjection_PerspectiveFov(float fovAngleY, float aspectRatio, float nearZ, float farZ)
	{
		*this = MMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
		return *this;
	}

	Matrix44& Matrix44::CreateProjection_Orthographic(float viewWidth, float viewHeight, float nearZ, float farZ)
	{
		*this = MatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
		return *this;
	}

	Matrix44& Matrix44::Inverse()
	{
		*this = MatrixInverse(nullptr, *this);
		return *this;
	}

	Matrix44& Matrix44::RotateX(float angle)
	{
		*this *= MatrixRotationX(angle);
		return *this;
	}

	Matrix44& Matrix44::RotateY(float angle)
	{
		*this *= MatrixRotationY(angle);
		return *this;
	}

	Matrix44& Matrix44::RotateZ(float angle)
	{
		*this *= MatrixRotationZ(angle);
		return *this;
	}

	Matrix44& Matrix44::RotateAxis(const Vector3& axis, float angle)
	{
		*this *= MatrixRotationAxis(Vec3Set(axis.Normalized()), angle);
		return *this;
	}

	Matrix44& Matrix44::Scale(float x, float y, float z)
	{
		*this = MatrixScaling(x, y, z);
		return *this;
	}

	Matrix44& Matrix44::Scale(const Vector3& v)
	{
		*this = MatrixScaling(v.x, v.y, v.z);
		return *this;
	}

	const Quaternion Matrix44::GetRotation() const
	{
		Vector3 scale, trans;
		Quaternion quat;

		if (!Decompose(scale, quat, trans))
		{
			return Quaternion();
		}

		return quat;
	}

	bool Matrix44::Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) const
	{
		MVector s, r, t;

		if (!MatrixDecompose(&s, &r, &t, *this))
		{
			return false;
		}

		scale = s;
		rotation = r;
		translation = t;
		return true;
	}

	Vector3 Matrix44::TransformPoint(const Vector3& v)const
	{
		return Vector3Transform(v, *this);
	}

	Vector3 Matrix44::TransformVector(const Vector3& v)const
	{
		return Vector3TransformNormal(v, *this);
	}

	Matrix44& Matrix44::Transpose()
	{
		*this = MatrixTranspose(*this);
		return *this;
	}

	Matrix44& Matrix44::LookAt(const Vector3& lookPosition, const Vector3& worldUp)
	{
		Vector3 tmp = Vec3Sub(Vec3Set(lookPosition), Vec3Set(GetPosition()));
		const auto tmpZ = tmp.Normalize();
		const auto X = Vector3::Up.Cross(tmpZ);
		const auto Y = worldUp.IsAllZero() ? tmpZ.Cross(X) : worldUp;//Y²‚ğŒÅ’è‚·‚é‚©”Û‚©.
		const auto Z = X.Cross(Y);

		float scaleX = GetXAxis().Length();
		float scaleY = GetYAxis().Length();
		float scaleZ = GetZAxis().Length();

		SetXAxis(X * scaleX);
		SetYAxis(Y * scaleY);
		SetZAxis(Z * scaleZ);

		return *this;
	}

	Matrix44& Matrix44::SetRotation(const Matrix44& matrix)
	{
		const auto OriginPosition = GetPosition();

		this->CreateScaling(GetScale());
		*this = MatrixMultiply(matrix, *this);
		return SetPosition(OriginPosition);
	}

	// Matrix“¯m‚Ì‡¬
	Matrix44 operator* (const Matrix44& M1, const Matrix44& M2)
	{
		return MatrixMultiply(M1, M2);
	}
}