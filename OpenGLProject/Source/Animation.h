#pragma once
#include <string>
#include <vector>
#include "Utility.h"
#include "BoneTransform.h"

class Animation {
 public:
  bool Load(const std::string& a_fileName);

  [[nodiscard]] const size_t GetNumBones() const { return mNumBones; }
  [[nodiscard]] const size_t GetNumFrames() const { return mNumFrames; }
  [[nodiscard]] const float GetDuration() const { return mDuration; }
  [[nodiscard]] const float GetFrameDuration() const { return mFrameDuration; }
  [[nodiscard]] const std::vector<std::vector<BoneTransform>>& GetTracks() const {return mTracks;}
  const size_t GetNowFrameClamped(float a_inTime) const {
    return std::clamp(static_cast<size_t>(a_inTime / mFrameDuration), static_cast<size_t>(0), static_cast<size_t>(mNumFrames - 1));
  }

  //! �n���ꂽ�z��ɃO���[�o���|�[�Y�s����L������
  //! �w�肳�ꂽ�����ɂ�����A�e�{�[���̌��݂�
  //! �O���[�o���s��ł���
  void GetGlobalPoseAtTime(std::vector<Matrix4>& a_outPoses,
                           const class Skeleton* a_inSkeleton,
                           float a_inTime) const;

 private:
  //! �A�j���[�V�����̃{�[����
  size_t mNumBones;
  //! �A�j���[�V�����̃t���[����
  size_t mNumFrames;
  //! �A�j���[�V�����̒���(�b��)
  float mDuration;
  //! �A�j���[�V�����̊e�t���[���̒���
  float mFrameDuration;
  //! �e�t���[���̕ϊ������g���b�N�Ɋi�[�F
  //! �O���̔z��̃C���f�b�N�X�̓{�[��
  //! �����̃C���f�b�N�X�̓t���[��
  std::vector<std::vector<BoneTransform>> mTracks;
};
