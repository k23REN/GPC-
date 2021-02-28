#pragma once
#include <string>
#include <vector>

#include "BoneTransform.h"

class Skeleton {
 public:
  //! �X�P���g���ɂ���X�̃{�[���̒�`
  struct Bone {
    BoneTransform mLocalBindPose;
    std::string mName;
    int mParent;
  };

  //! �t�@�C���Ǎ�
  bool Load(const std::string& fileName);

  [[nodiscard]] size_t GetNumBones() const { return m_bones.size(); }
  [[nodiscard]] const Bone& GetBone(size_t a_idx) const { return m_bones[a_idx]; }
  [[nodiscard]] const Bone& GetBone(const std::string& a_boneName) const;
  [[nodiscard]] const std::vector<Bone>& GetBones() const { return m_bones; }
  [[nodiscard]] const std::vector<Matrix4>& GetGlobalInvBindPoses() const {
    return m_globalInvBindPoses;
  }

 protected:
  /**
   * @fn
   * �S�Ẵ{�[���̃O���[�o���ȋt�o�C���h�|�[�Y�s����v�Z
   * (�X�P���g���̃��[�h���ɌĂяo�����)
   * @param �Ȃ�
   * @return �Ȃ�
   */
  void ComputeGlobalInvBindPose();

 private:
  //! �X�P���g���̃{�[���z��
  std::vector<Bone> m_bones;
  //! �{�[���̃O���[�o���ȋt�o�C���h�|�[�Y�s��
  std::vector<Matrix4> m_globalInvBindPoses;
};
