#pragma once
#include <random>

#include "Math.h"

class Random {
 public:
  static void Init();

  /**
   * @fn
   * �w�肳�ꂽint�^�̒l�ŃV�[�h��ݒ�
   * ���ʏ�̏ꍇ�A�蓮�Őݒ肷��K�v�͂Ȃ�
   * @brief �V�[�h��ݒ�
   * @param (a_seed) �V�[�h�l
   * @return �Ȃ�
   */
  static void Seed(unsigned int a_seed);

  /**
   * @fn
   * 0.0f ���� 1.0f �̊Ԃ�float�^�����_���l���擾
   * @brief 0.0f�`1.0f�̒l���擾
   * @return 0.0f�`1.0f��float�^�����_���l
   */
  [[nodiscard]] static float GetFloat();

  /**
   * @fn
   * a_min ���� a_max �̊Ԃ�float�^�����_���l���擾
   * @brief a_min�`a_max�̒l���擾
   * @param (a_min) �ŏ��l
   * @param (a_max) �ő�l
   * @return a_min�`a_max��float�^�����_���l
   */
  [[nodiscard]] static float GetFloatRange(float a_min, float a_max);

  /**
   * @fn
   * a_min ���� a_max �̊Ԃ�int�^�����_���l���擾
   * @brief a_min�`a_max�̒l���擾
   * @param (a_min) �ŏ��l
   * @param (a_max) �ő�l
   * @return a_min�`a_max��int�^�����_���l
   */
  [[nodiscard]] static int GetIntRange(int a_min, int a_max);

  /**
   * @fn
   * a_min ���� a_max �̊Ԃ�Vector2�^�����_���l���擾
   * @brief a_min�`a_max�̒l���擾
   * @param (a_min) �ŏ��l
   * @param (a_max) �ő�l
   * @return a_min�`a_max��Vector2�^�����_���l
   */
  [[nodiscard]] static Vector2 GetVector(const Vector2& a_min,
                                         const Vector2& a_max);

    /**
   * @fn
   * a_min ���� a_max �̊Ԃ�Vector3�^�����_���l���擾
   * @brief a_min�`a_max�̒l���擾
   * @param (a_min) �ŏ��l
   * @param (a_max) �ő�l
   * @return a_min�`a_max��Vector3�^�����_���l
   */
  [[nodiscard]] static Vector3 GetVector(const Vector3& a_min,
                                         const Vector3& a_max);

 private:
  static std::mt19937 sGenerator;
};