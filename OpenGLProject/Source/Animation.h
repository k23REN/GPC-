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

  //! 渡された配列にグローバルポーズ行列を記入する
  //! 指定された時刻における、各ボーンの現在の
  //! グローバル行列である
  void GetGlobalPoseAtTime(std::vector<Matrix4>& a_outPoses,
                           const class Skeleton* a_inSkeleton,
                           float a_inTime) const;

 private:
  //! アニメーションのボーン数
  size_t mNumBones;
  //! アニメーションのフレーム数
  size_t mNumFrames;
  //! アニメーションの長さ(秒数)
  float mDuration;
  //! アニメーションの各フレームの長さ
  float mFrameDuration;
  //! 各フレームの変換情報をトラックに格納：
  //! 外側の配列のインデックスはボーン
  //! 内側のインデックスはフレーム
  std::vector<std::vector<BoneTransform>> mTracks;
};
