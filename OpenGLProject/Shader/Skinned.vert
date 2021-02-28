#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
uniform mat4 uMatrixPalette[96];

// ����
// 0�F�ʒu, 1 �@��,
// 2�F�{�[���̏���, 3�F�d��,
// 4�F�e�N�X�`�����W.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in uvec4 inSkinBones;
layout(location = 3) in vec4 inSkinWeights;
layout(location = 4) in vec2 inTexCoord;

out vec2 fragTexCoord;
// �@��
out vec3 fragNormal;
// ���[���h���W
out vec3 fragWorldPos;

void main()
{
	// �ʒu�𓯎����W�ɕϊ�
	vec4 pos = vec4(inPosition, 1.0);
	
	// �ʒu�̃X�L�j���O
	vec4 skinnedPos = (pos * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// �ʒu�����[���h��Ԃɕϊ�
	skinnedPos = skinnedPos * uWorldTransform;
	// ���[���h�|�W�V������ۑ�
	fragWorldPos = skinnedPos.xyz;
	// �N���b�v��Ԃɕϊ�
	gl_Position = skinnedPos * uViewProj;

	// ���_�@���̃X�L�j���O
	vec4 skinnedNormal = vec4(inNormal, 0.0f);
	skinnedNormal = (skinnedNormal * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x
		+ (skinnedNormal * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y
		+ (skinnedNormal * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z
		+ (skinnedNormal * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;
	// �@�������[���h��Ԃɕϊ�(w=0)
	fragNormal = (skinnedNormal * uWorldTransform).xyz;

	// �e�N�X�`�����W���t���O�����g�V�F�[�_�[�ɓn��
	fragTexCoord = inTexCoord;
}

