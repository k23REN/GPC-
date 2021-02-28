#include "BoneTransform.h"

Matrix4 BoneTransform::ToMatrix() const
{
	Matrix4 rot = Matrix4::CreateFromQuaternion(m_rotation);
	Matrix4 trans = Matrix4::CreateTranslation(m_translation);

	return rot * trans;
}

BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
	BoneTransform retVal;
	retVal.m_rotation = Quaternion::Slerp(a.m_rotation, b.m_rotation, f);
	retVal.m_translation = Vector3::Lerp(a.m_translation, b.m_translation, f);
	return retVal;
}
