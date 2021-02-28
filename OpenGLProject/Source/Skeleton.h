#pragma once
#include <string>
#include <vector>

#include "BoneTransform.h"

class Skeleton {
 public:
  //! スケルトンにある個々のボーンの定義
  struct Bone {
    BoneTransform mLocalBindPose;
    std::string mName;
    int mParent;
  };

  //! ファイル読込
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
   * 全てのボーンのグローバルな逆バインドポーズ行列を計算
   * (スケルトンのロード時に呼び出される)
   * @param なし
   * @return なし
   */
  void ComputeGlobalInvBindPose();

 private:
  //! スケルトンのボーン配列
  std::vector<Bone> m_bones;
  //! ボーンのグローバルな逆バインドポーズ行列
  std::vector<Matrix4> m_globalInvBindPoses;
};
