#include "Skeleton.h"

#include <SDL/SDL_log.h>
#include <rapidjson/document.h>

#include <fstream>
#include <sstream>

#include "MatrixPalette.h"

bool Skeleton::Load(const std::string& fileName) {
  std::ifstream file(fileName);
  if (!file.is_open()) {
    SDL_Log("File not found: Skeleton %s", fileName.c_str());
    return false;
  }

  std::stringstream fileStream;
  fileStream << file.rdbuf();
  std::string contents = fileStream.str();
  rapidjson::StringStream jsonStr(contents.c_str());
  rapidjson::Document doc;
  doc.ParseStream(jsonStr);

  if (!doc.IsObject()) {
    SDL_Log("Skeleton %s is not valid json", fileName.c_str());
    return false;
  }

  int ver = doc["version"].GetInt();

  // Check the metadata
  if (ver != 1) {
    SDL_Log("Skeleton %s unknown format", fileName.c_str());
    return false;
  }

  const rapidjson::Value& bonecount = doc["bonecount"];
  if (!bonecount.IsUint()) {
    SDL_Log("Skeleton %s doesn't have a bone count.", fileName.c_str());
    return false;
  }

  size_t count = bonecount.GetUint();

  if (count > MAX_SKELETON_BONES) {
    SDL_Log("Skeleton %s exceeds maximum bone count.", fileName.c_str());
    return false;
  }

  m_bones.reserve(count);

  const rapidjson::Value& bones = doc["bones"];
  if (!bones.IsArray()) {
    SDL_Log("Skeleton %s doesn't have a bone array?", fileName.c_str());
    return false;
  }

  if (bones.Size() != count) {
    SDL_Log(
        "Skeleton %s has a mismatch between the bone count and number of bones",
        fileName.c_str());
    return false;
  }

  Bone temp;

  for (rapidjson::SizeType i = 0; i < count; i++) {
    if (!bones[i].IsObject()) {
      SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
      return false;
    }

    const rapidjson::Value& name = bones[i]["name"];
    temp.mName = name.GetString();

    const rapidjson::Value& parent = bones[i]["parent"];
    temp.mParent = parent.GetInt();

    const rapidjson::Value& bindpose = bones[i]["bindpose"];
    if (!bindpose.IsObject()) {
      SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
      return false;
    }

    const rapidjson::Value& rot = bindpose["rot"];
    const rapidjson::Value& trans = bindpose["trans"];

    if (!rot.IsArray() || !trans.IsArray()) {
      SDL_Log("Skeleton %s: Bone %d is invalid.", fileName.c_str(), i);
      return false;
    }

    temp.mLocalBindPose.m_rotation.x = static_cast<float>(rot[0].GetDouble());
    temp.mLocalBindPose.m_rotation.y = static_cast<float>(rot[1].GetDouble());
    temp.mLocalBindPose.m_rotation.z = static_cast<float>(rot[2].GetDouble());
    temp.mLocalBindPose.m_rotation.w = static_cast<float>(rot[3].GetDouble());

    temp.mLocalBindPose.m_translation.x = static_cast<float>(trans[0].GetDouble());
    temp.mLocalBindPose.m_translation.y = static_cast<float>(trans[1].GetDouble());
    temp.mLocalBindPose.m_translation.z = static_cast<float>(trans[2].GetDouble());

    m_bones.emplace_back(temp);
  }

  // Now that we have the bones
  ComputeGlobalInvBindPose();

  return true;
}

const Skeleton::Bone& Skeleton::GetBone(const std::string& a_boneName) const {
  for (const auto& bone : m_bones) {
    if (bone.mName == a_boneName) {
      return bone;
    }
  }
}

void Skeleton::ComputeGlobalInvBindPose() {
  //! ボーンの数で配列の要素数を変更する。自動的にIDが記入される
  m_globalInvBindPoses.resize(GetNumBones());

  //! Step 1: 各ボーンのグローバルバインドポーズを計算

  //! ルートのグローバルポーズだけはローカルバインドポーズと同じ
  m_globalInvBindPoses[0] = m_bones[0].mLocalBindPose.ToMatrix();

  //! 残りのボーンのグローバルバインドポーズは、
  //! ローカルポーズに親のグローバルポーズをかけたもの
  for (size_t i = 1; i < m_globalInvBindPoses.size(); i++) {
    Matrix4 localMat = m_bones[i].mLocalBindPose.ToMatrix();
    m_globalInvBindPoses[i] = localMat * m_globalInvBindPoses[m_bones[i].mParent];
  }

  // Step 2: 角行列の逆行列を求める
  for (size_t i = 0; i < m_globalInvBindPoses.size(); i++) {
    m_globalInvBindPoses[i].Invert();
  }
}
