#include "Shader.h"
#include<SDL/SDL.h>
#include<fstream>
#include<sstream>

bool Shader::Load(std::string_view a_vertName, std::string_view a_fragName) 
{
  //シェーダをコンパイル
  if (!CompileShader(a_vertName, GL_VERTEX_SHADER, m_vertexShader) ||
      !CompileShader(a_fragName, GL_FRAGMENT_SHADER, m_fragShader)) {
    return false;
  }

  //シェーダをリンクさせる
  m_shaderProgram = glCreateProgram();
  glAttachShader(m_shaderProgram, m_vertexShader);
  glAttachShader(m_shaderProgram, m_fragShader);
  glLinkProgram(m_shaderProgram);

  //リンクが成功したかどうか確認
  if (!IsValidProgram()) {
    return false;
  }

  return true;
}

void Shader::Unload() 
{
  glDeleteProgram(m_shaderProgram);
  glDeleteShader(m_vertexShader);
  glDeleteShader(m_fragShader);
}

void Shader::SetActive() 
{ 
  glUseProgram(m_shaderProgram);
}

bool Shader::CompileShader(std::string_view a_fileName, GLenum a_shaderType,
                           GLuint& a_outShader) 
{
  std::ifstream shaderFile(a_fileName.data());

  if (shaderFile.is_open()) {
    //読み込んだファイルを1つの文字列にする
    std::stringstream sstream;
    sstream << shaderFile.rdbuf();
    std::string contents = sstream.str();
    const char* contentsChar = contents.c_str();

    //指定されたタイプのシェーダを生成
    a_outShader = glCreateShader(a_shaderType);
    //読み鋳込んだ文字列でコンパイルする
    glShaderSource(a_outShader, 1, &(contentsChar), nullptr);
    glCompileShader(a_outShader);

    if (!IsCompiled(a_outShader)) {  //コンパイルに失敗した場合
      SDL_Log("Failed to compile shader %s", a_fileName.data());
      return false;
    }
  } else {  //ファイルがない場合
    SDL_Log("Shader file not found: %s", a_fileName.data());
    return false;
  }
  return true;
}

bool Shader::IsCompiled(GLuint a_shader) 
{
  GLint status;
  //コンパイルの状態を確認
  glGetShaderiv(a_shader, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE) {
    char buffer[512];
    memset(buffer, 0, 512);
    glGetShaderInfoLog(a_shader, 511, nullptr, buffer);
    SDL_Log("GLSL Compile Failed:\n%s", buffer);
    return false;
  }

  return true;
}

bool Shader::IsValidProgram() 
{ 
   	GLint status;
  //リンクの状態を確認
  glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    char buffer[512];
    memset(buffer, 0, 512);
    glGetProgramInfoLog(m_shaderProgram, 511, nullptr, buffer);
    SDL_Log("GLSL Link Status:\n%s", buffer);
    return false;
  }

  return true;
}

void Shader::SetMatrixUniform(const char* a_name, const Matrix4& a_matrix) 
{
  //a_nameに対応したuniformを探す
  GLuint loc = glGetUniformLocation(m_shaderProgram, a_name);

  //行列データを探したuniformに送る
  glUniformMatrix4fv(loc, 1, GL_TRUE, a_matrix.GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* a_name, const Vector3& a_vector)
{
    GLuint loc = glGetUniformLocation(m_shaderProgram, a_name);
    //ベクトルデータを探したuniformに送る
    glUniform3fv(loc, 1, a_vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char* a_name, float a_value)
{
    GLuint loc = glGetUniformLocation(m_shaderProgram, a_name);
    //floatデータを探したuniformに送る
    glUniform1f(loc, a_value);
}
