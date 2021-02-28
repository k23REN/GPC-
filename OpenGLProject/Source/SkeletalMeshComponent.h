#pragma once
#include "Actor.h"
#include "Animation.h"
#include "MatrixPalette.h"
#include "MeshComponent.h"
#include "Skeleton.h"

class SkeletalMeshComponent : public MeshComponent {
 public:
  SkeletalMeshComponent(Actor* a_owner, Mesh* a_mesh);
  //! ���b�V���R���|�[�l���g�̕`��
  void Draw(Sptr<Shader> a_shader) override;

  void Update(float a_deltaTime) override;

  Vector3 GetBonePosition(const std::string& a_boneName);

  void SetSkeleton(const class Skeleton* a_sk) { m_pSkeleton = a_sk; }

  float PlayAnimation(const class Animation* a_anim, float a_playRate = 1.0f, bool a_isBlend = false);

 protected:
  void ComputeMatrixPalette();

  //! �s��p���b�g
  MatrixPalette m_palette;
  const Skeleton* m_pSkeleton;
  //! ���Đ����Ă���A�j���[�V����
  const Animation* m_pAnimation = nullptr;
  const Animation* m_pSecondAnimation = nullptr;
  //! �A�j���[�V�����Đ����[�g(1.0���)
  float m_animPlayRate = 1.0f;
  //! �A�j���[�V�����̌��ݎ���
  float m_animTime = 0.0f;
  float m_secondAnimTime = 0.0f;
  //! �A�j���[�V�����̃u�����h
  float m_blendTime = 0.0f;

  //! ���݂̃|�[�Y�s��
  std::vector<Matrix4> m_nowAnimPoses;
};
