#pragma once
#include <string>
#include <GL/glew.h>
#include "Math.h"

class Shader
{
 public:

  /**
   * @fn
   * �w�肳�ꂽ���O�̃V�F�[�_��ǂݍ���
   * @brief �V�F�[�_�ǂݍ���
   * @param (a_vertName) ���_�V�F�[�_��
   * @param (a_fragName) �t���O�����g�V�F�[�_��
   * @return �ǂݍ��ݐ���
   */
  bool Load(std::string_view a_vertName, std::string_view a_fragName);

  /**
   * @fn
   * �V�F�[�_���폜����
   * @return �Ȃ�
   */
  void Unload();

  /**
   * @fn
   * �V�F�[�_���A�N�e�B�u�ɂ���
   * @return �Ȃ�
   */
  void SetActive();

    /**
   * @fn
   * �s����V�F�[�_�ɓn��
   * @return �Ȃ�
   */
  void SetMatrixUniform(const char* a_name, const Matrix4& a_matrix);
  void SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count);
  /**
  * @fn
  * �x�N�g�����V�F�[�_�ɓn��
  * @return �Ȃ�
  */
  void SetVectorUniform(const char* name, const Vector3& vector);

  /**
 * @fn
 * float�̒l���V�F�[�_�ɓn��
 * @return �Ȃ�
 */
  void SetFloatUniform(const char* name, float value);

 private:
  /**
   * @fn
   * �V�F�[�_���R���p�C������
   * @return �R���p�C������
   */
  bool CompileShader(std::string_view a_fileName, GLenum a_shaderType,
                     GLuint& a_outShader);

  /**
   * @fn
   * �V�F�[�_���R���p�C����������������
   * @return �R���p�C������
   */
  bool IsCompiled(GLuint a_shader);

  /**
   * @fn
   * �V�F�[�_�v���O�����̃����N�m�F
   * @return �����N���Ă���
   */
  bool IsValidProgram();

  //!�V�F�[�_��ID���i�[
  GLuint m_vertexShader;
  GLuint m_fragShader;
  GLuint m_shaderProgram;
};