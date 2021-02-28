#include "Animation.h"

#include <SDL/SDL_log.h>
#include <rapidjson/document.h>

#include <fstream>
#include <sstream>

#include "Skeleton.h"

bool Animation::Load(const std::string& a_fileName) {
  std::ifstream file(a_fileName);
  if (!file.is_open()) {
    SDL_Log("File not found: Animation %s", a_fileName.c_str());
    return false;
  }

  std::stringstream fileStream;
  fileStream << file.rdbuf();
  std::string contents = fileStream.str();
  rapidjson::StringStream jsonStr(contents.c_str());
  rapidjson::Document doc;
  doc.ParseStream(jsonStr);

  if (!doc.IsObject()) {
    SDL_Log("Animation %s is not valid json", a_fileName.c_str());
    return false;
  }

  int ver = doc["version"].GetInt();

  // Check the metadata
  if (ver != 1) {
    SDL_Log("Animation %s unknown format", a_fileName.c_str());
    return false;
  }

  const rapidjson::Value& sequence = doc["sequence"];
  if (!sequence.IsObject()) {
    SDL_Log("Animation %s doesn't have a sequence.", a_fileName.c_str());
    return false;
  }

  const rapidjson::Value& frames = sequence["frames"];
  const rapidjson::Value& length = sequence["length"];
  const rapidjson::Value& bonecount = sequence["bonecount"];

  if (!frames.IsUint() || !length.IsDouble() || !bonecount.IsUint()) {
    SDL_Log("Sequence %s has invalid frames, length, or bone count.",
            a_fileName.c_str());
    return false;
  }

  mNumFrames = frames.GetUint();
  mDuration = static_cast<float>(length.GetDouble());
  mNumBones = bonecount.GetUint();
  mFrameDuration = mDuration / (mNumFrames - 1);

  mTracks.resize(mNumBones);

  const rapidjson::Value& tracks = sequence["tracks"];

  if (!tracks.IsArray()) {
    SDL_Log("Sequence %s missing a tracks array.", a_fileName.c_str());
    return false;
  }

  for (rapidjson::SizeType i = 0; i < tracks.Size(); i++) {
    if (!tracks[i].IsObject()) {
      SDL_Log("Animation %s: Track element %d is invalid.", a_fileName.c_str(),
              i);
      return false;
    }

    size_t boneIndex = tracks[i]["bone"].GetUint();

    const rapidjson::Value& transforms = tracks[i]["transforms"];
    if (!transforms.IsArray()) {
      SDL_Log("Animation %s: Track element %d is missing transforms.",
              a_fileName.c_str(), i);
      return false;
    }

    BoneTransform temp;

    if (transforms.Size() < mNumFrames) {
      SDL_Log("Animation %s: Track element %d has fewer frames than expected.",
              a_fileName.c_str(), i);
      return false;
    }

    for (rapidjson::SizeType j = 0; j < transforms.Size(); j++) {
      const rapidjson::Value& rot = transforms[j]["rot"];
      const rapidjson::Value& trans = transforms[j]["trans"];

      if (!rot.IsArray() || !trans.IsArray()) {
        SDL_Log("Skeleton %s: Bone %d is invalid.", a_fileName.c_str(), i);
        return false;
      }

      temp.m_rotation.x = static_cast<float>(rot[0].GetDouble());
      temp.m_rotation.y = static_cast<float>(rot[1].GetDouble());
      temp.m_rotation.z = static_cast<float>(rot[2].GetDouble());
      temp.m_rotation.w = static_cast<float>(rot[3].GetDouble());

      temp.m_translation.x = static_cast<float>(trans[0].GetDouble());
      temp.m_translation.y = static_cast<float>(trans[1].GetDouble());
      temp.m_translation.z = static_cast<float>(trans[2].GetDouble());

      mTracks[boneIndex].emplace_back(temp);
    }
  }

  return true;
}

void Animation::GetGlobalPoseAtTime(std::vector<Matrix4>& a_outPoses,
                                    const Skeleton* a_inSkeleton,
                                    float a_inTime) const {
  //! 必要ならばoutPoses配列の要素数を再設定する
  if (a_outPoses.size() != mNumBones) {
    a_outPoses.resize(mNumBones);
  }

  //! 現在と次のフレームインデックスを求める
  //! (inTimeの範囲を[0, AnimDuration]と想定する)
  size_t frame = static_cast<size_t>(a_inTime / mFrameDuration);

  //! 最後のフレームは補間しない
  if (frame >= (mNumFrames)) {
    //! ルートのポーズを設定
    if (mTracks[0].size() > 0) {
      a_outPoses[0] = mTracks[0][frame].ToMatrix();
    } else {
      a_outPoses[0] = Matrix4::Identity;
    }

    const std::vector<Skeleton::Bone>& bones = a_inSkeleton->GetBones();
    //! 残りのポーズを設定する
    for (size_t bone = 1; bone < mNumBones; bone++) {
      Matrix4 localMat;  //! (デフォルトでは単位行列)
      if (mTracks[bone].size() > 0) {
        localMat = mTracks[bone][frame].ToMatrix();
      }

      a_outPoses[bone] = localMat * a_outPoses[bones[bone].mParent];
    }
  } else { //! 補間が必要なフレーム
    size_t nextFrame = frame + 1;
    //! frameとnextframeの間の小数値を計算
    float pct = a_inTime / mFrameDuration - frame;

    //! ルートのポーズを設定
    if (mTracks[0].size() > 0) {
      //! 現在のフレームのポーズと次のフレームとの間で補間する
      BoneTransform interp = BoneTransform::Interpolate(
          mTracks[0][frame], mTracks[0][nextFrame], pct);
      a_outPoses[0] = interp.ToMatrix();
    } else {
      a_outPoses[0] = Matrix4::Identity;
    }

    const std::vector<Skeleton::Bone>& bones = a_inSkeleton->GetBones();
    //! 残りのポーズを設定する
    for (size_t bone = 1; bone < mNumBones; bone++) {
      Matrix4 localMat;  //! (デフォルトでは単位行列)
      if (mTracks[bone].size() > 0) {
        BoneTransform interp = BoneTransform::Interpolate(
            mTracks[bone][frame], mTracks[bone][nextFrame], pct);
        localMat = interp.ToMatrix();
      }

      a_outPoses[bone] = localMat * a_outPoses[bones[bone].mParent];
    }
  }


}
