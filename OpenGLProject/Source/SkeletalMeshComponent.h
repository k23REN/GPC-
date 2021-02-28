#pragma once
#include "Actor.h"
#include "Animation.h"
#include "MatrixPalette.h"
#include "MeshComponent.h"
#include "Skeleton.h"

class SkeletalMeshComponent : public MeshComponent {
 public:
  SkeletalMeshComponent(Actor* a_owner, Mesh* a_mesh);
  //! メッシュコンポーネントの描画
  void Draw(Sptr<Shader> a_shader) override;

  void Update(float a_deltaTime) override;

  Vector3 GetBonePosition(const std::string& a_boneName);

  void SetSkeleton(const class Skeleton* a_sk) { m_pSkeleton = a_sk; }

  float PlayAnimation(const class Animation* a_anim, float a_playRate = 1.0f, bool a_isBlend = false);

 protected:
  void ComputeMatrixPalette();

  //! 行列パレット
  MatrixPalette m_palette;
  const Skeleton* m_pSkeleton;
  //! 今再生しているアニメーション
  const Animation* m_pAnimation = nullptr;
  const Animation* m_pSecondAnimation = nullptr;
  //! アニメーション再生レート(1.0が基準)
  float m_animPlayRate = 1.0f;
  //! アニメーションの現在時刻
  float m_animTime = 0.0f;
  float m_secondAnimTime = 0.0f;
  //! アニメーションのブレンド
  float m_blendTime = 0.0f;

  //! 現在のポーズ行列
  std::vector<Matrix4> m_nowAnimPoses;
};
