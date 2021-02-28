#pragma once
#include <string>
#include <GL/glew.h>
#include "Math.h"

class Shader
{
 public:

  /**
   * @fn
   * 指定された名前のシェーダを読み込む
   * @brief シェーダ読み込み
   * @param (a_vertName) 頂点シェーダ名
   * @param (a_fragName) フラグメントシェーダ名
   * @return 読み込み成功
   */
  bool Load(std::string_view a_vertName, std::string_view a_fragName);

  /**
   * @fn
   * シェーダを削除する
   * @return なし
   */
  void Unload();

  /**
   * @fn
   * シェーダをアクティブにする
   * @return なし
   */
  void SetActive();

    /**
   * @fn
   * 行列をシェーダに渡す
   * @return なし
   */
  void SetMatrixUniform(const char* a_name, const Matrix4& a_matrix);
  void SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count);
  /**
  * @fn
  * ベクトルをシェーダに渡す
  * @return なし
  */
  void SetVectorUniform(const char* name, const Vector3& vector);

  /**
 * @fn
 * floatの値をシェーダに渡す
 * @return なし
 */
  void SetFloatUniform(const char* name, float value);

 private:
  /**
   * @fn
   * シェーダをコンパイルする
   * @return コンパイル成功
   */
  bool CompileShader(std::string_view a_fileName, GLenum a_shaderType,
                     GLuint& a_outShader);

  /**
   * @fn
   * シェーダがコンパイル成功したか判定
   * @return コンパイル成功
   */
  bool IsCompiled(GLuint a_shader);

  /**
   * @fn
   * シェーダプログラムのリンク確認
   * @return リンクしている
   */
  bool IsValidProgram();

  //!シェーダのIDを格納
  GLuint m_vertexShader;
  GLuint m_fragShader;
  GLuint m_shaderProgram;
};