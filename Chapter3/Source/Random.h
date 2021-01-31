#pragma once
#include <random>

#include "Math.h"

class Random {
 public:
  static void Init();

  /**
   * @fn
   * 指定されたint型の値でシードを設定
   * ※通常の場合、手動で設定する必要はない
   * @brief シードを設定
   * @param (a_seed) シード値
   * @return なし
   */
  static void Seed(unsigned int a_seed);

  /**
   * @fn
   * 0.0f から 1.0f の間のfloat型ランダム値を取得
   * @brief 0.0f～1.0fの値を取得
   * @return 0.0f～1.0fのfloat型ランダム値
   */
  [[nodiscard]] static float GetFloat();

  /**
   * @fn
   * a_min から a_max の間のfloat型ランダム値を取得
   * @brief a_min～a_maxの値を取得
   * @param (a_min) 最小値
   * @param (a_max) 最大値
   * @return a_min～a_maxのfloat型ランダム値
   */
  [[nodiscard]] static float GetFloatRange(float a_min, float a_max);

  /**
   * @fn
   * a_min から a_max の間のint型ランダム値を取得
   * @brief a_min～a_maxの値を取得
   * @param (a_min) 最小値
   * @param (a_max) 最大値
   * @return a_min～a_maxのint型ランダム値
   */
  [[nodiscard]] static int GetIntRange(int a_min, int a_max);

  /**
   * @fn
   * a_min から a_max の間のVector2型ランダム値を取得
   * @brief a_min～a_maxの値を取得
   * @param (a_min) 最小値
   * @param (a_max) 最大値
   * @return a_min～a_maxのVector2型ランダム値
   */
  [[nodiscard]] static Vector2 GetVector(const Vector2& a_min,
                                         const Vector2& a_max);

    /**
   * @fn
   * a_min から a_max の間のVector3型ランダム値を取得
   * @brief a_min～a_maxの値を取得
   * @param (a_min) 最小値
   * @param (a_max) 最大値
   * @return a_min～a_maxのVector3型ランダム値
   */
  [[nodiscard]] static Vector3 GetVector(const Vector3& a_min,
                                         const Vector3& a_max);

 private:
  static std::mt19937 sGenerator;
};