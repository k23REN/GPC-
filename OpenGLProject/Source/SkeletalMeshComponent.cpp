#include "SkeletalMeshComponent.h"

#include "Game.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* a_owner, Mesh* a_mesh)
    : MeshComponent(a_owner, a_mesh, true), m_pSkeleton(nullptr) {}

void SkeletalMeshComponent::Draw(Sptr<Shader> a_shader) {
  if (m_pMesh) {
    //! ���[���h���W�ݒ�
    a_shader->SetMatrixUniform("uWorldTransform",
                               m_pOwner->GetWorldTransform());
    //! �s��p���b�g�ݒ�
    a_shader->SetMatrixUniforms("uMatrixPalette", &m_palette.mEntry[0],
                                MAX_SKELETON_BONES);
    a_shader->SetFloatUniform("uSpecPower", m_pMesh->GetSpecPower());
    Sptr<Texture> t = m_pMesh->GetTexture(m_textureIndex);
    if (t) {
      t->SetActive();
    }

    Sptr<VertexArray> va = m_pMesh->GetVertexArray();
    va->SetActive();
    //! �`��
    glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
  }
}

void SkeletalMeshComponent::Update(float deltaTime) {
  if (m_pAnimation && m_pSkeleton) {
    if (m_pSecondAnimation) {
      //! �u�����h���鎞��
      if (m_blendTime >= 1.0f) {
        if (m_pSecondAnimation) {
          m_pAnimation = m_pSecondAnimation;
          m_animTime = m_secondAnimTime;
          m_pSecondAnimation = nullptr;
          m_secondAnimTime = 0.0f;
        }
      } else { //�u�����h��
        m_blendTime += deltaTime * m_animPlayRate;
        //! �u�����h�������̃A�j���[�V������i�߂�
        m_secondAnimTime += deltaTime * m_animPlayRate;

            //! �����𒴂����犪���߂�
        while (m_secondAnimTime > m_pSecondAnimation->GetDuration()) {
          m_secondAnimTime -= m_pSecondAnimation->GetDuration();
        }

      }
    } else {
      m_animTime += deltaTime * m_animPlayRate;
    }
    //! �����𒴂����犪���߂�
    while (m_animTime > m_pAnimation->GetDuration()) {
      m_animTime -= m_pAnimation->GetDuration();
    }

    //! �s��p���b�g���Čv�Z
    ComputeMatrixPalette();
  }
}

Vector3 SkeletalMeshComponent::GetBonePosition(
    const std::string& a_boneName) {
  Matrix4 mat = m_pSkeleton->GetBone(a_boneName).mLocalBindPose.ToMatrix() *
                m_nowAnimPoses[m_pSkeleton->GetBone(a_boneName).mParent];

  return mat.GetTranslation();
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim,
                                           float playRate, bool a_isBlend) {
  if (a_isBlend) {
    m_pSecondAnimation = anim;
    m_secondAnimTime = 0.0f;
    m_blendTime = 0.0f;
    m_animPlayRate = playRate;

    if (!m_pSecondAnimation) {
      return 0.0f;
    }

    ComputeMatrixPalette();

    return m_pSecondAnimation->GetDuration();
  } else {
    m_pAnimation = anim;
    m_animTime = 0.0f;
    m_animPlayRate = playRate;

    if (!m_pAnimation) {
      return 0.0f;
    }

    ComputeMatrixPalette();

    return m_pAnimation->GetDuration();
  }

  //�����͒ʂ�Ȃ���warning�ɂȂ�Ȃ��悤�ɋL�q
  return 0.0f;
}

void SkeletalMeshComponent::ComputeMatrixPalette() {
  const std::vector<Matrix4>& globalInvBindPoses =
      m_pSkeleton->GetGlobalInvBindPoses();
  std::vector<Matrix4> currentPoses;
  m_pAnimation->GetGlobalPoseAtTime(currentPoses, m_pSkeleton, m_animTime);
  m_nowAnimPoses = currentPoses;

  if (m_pSecondAnimation) {
    std::vector<Matrix4> secondCurrentPoses;
    m_pSecondAnimation->GetGlobalPoseAtTime(secondCurrentPoses, m_pSkeleton,
                                            m_secondAnimTime);

    //! �e�{�[���̃p���b�g��ݒ�
    for (size_t i = 0; i < m_pSkeleton->GetNumBones(); i++) {
      const Vector3 blendedPos = Vector3::Lerp(currentPoses[i].GetTranslation(), secondCurrentPoses[i].GetTranslation(),m_blendTime);
      const Quaternion blendedQuat = Quaternion::Slerp(
          Quaternion::MatrixToQuaternion(currentPoses[i]),
          Quaternion::MatrixToQuaternion(secondCurrentPoses[i] ),
                                                      m_blendTime);

      const Matrix4 blendedMat = Matrix4::CreateFromSRT(
          Vector3(1.0f, 1.0f, 1.0f),
          Quaternion::Normalize(blendedQuat),
          blendedPos);
      //const Matrix4 blendedMat = Matrix4::CreateFromTRS(blendedPos,
      //                           Quaternion::Normalize(blendedQuat),Vector3(1.0f, 1.0f, 1.0f)
      //    );

      //! �O���[�o���ȋt�o�C���h�|�[�Y�s��ƌ��݂̃|�[�Y�s��̐�
      m_palette.mEntry[i] = globalInvBindPoses[i] * blendedMat;
    }
  } else {
      //! �e�{�[���̃p���b�g��ݒ�
    for (size_t i = 0; i < m_pSkeleton->GetNumBones(); i++) {
      //! �O���[�o���ȋt�o�C���h�|�[�Y�s��ƌ��݂̃|�[�Y�s��̐�
      m_palette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
    }
  }



}
