#include "Shader.h"
#include<SDL/SDL.h>
#include<fstream>
#include<sstream>

bool Shader::Load(std::string_view a_vertName, std::string_view a_fragName) 
{
  //�V�F�[�_���R���p�C��
  if (!CompileShader(a_vertName, GL_VERTEX_SHADER, m_vertexShader) ||
      !CompileShader(a_fragName, GL_FRAGMENT_SHADER, m_fragShader)) {
    return false;
  }

  //�V�F�[�_�������N������
  m_shaderProgram = glCreateProgram();
  glAttachShader(m_shaderProgram, m_vertexShader);
  glAttachShader(m_shaderProgram, m_fragShader);
  glLinkProgram(m_shaderProgram);

  //�����N�������������ǂ����m�F
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
    //�ǂݍ��񂾃t�@�C����1�̕�����ɂ���
    std::stringstream sstream;
    sstream << shaderFile.rdbuf();
    std::string contents = sstream.str();
    const char* contentsChar = contents.c_str();

    //�w�肳�ꂽ�^�C�v�̃V�F�[�_�𐶐�
    a_outShader = glCreateShader(a_shaderType);
    //�ǂݒ����񂾕�����ŃR���p�C������
    glShaderSource(a_outShader, 1, &(contentsChar), nullptr);
    glCompileShader(a_outShader);

    if (!IsCompiled(a_outShader)) {  //�R���p�C���Ɏ��s�����ꍇ
      SDL_Log("Failed to compile shader %s", a_fileName.data());
      return false;
    }
  } else {  //�t�@�C�����Ȃ��ꍇ
    SDL_Log("Shader file not found: %s", a_fileName.data());
    return false;
  }
  return true;
}

bool Shader::IsCompiled(GLuint a_shader) 
{
  GLint status;
  //�R���p�C���̏�Ԃ��m�F
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
  //�����N�̏�Ԃ��m�F
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
  //a_name�ɑΉ�����uniform��T��
  GLuint loc = glGetUniformLocation(m_shaderProgram, a_name);

  //�s��f�[�^��T����uniform�ɑ���
  glUniformMatrix4fv(loc, 1, GL_TRUE, a_matrix.GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* a_name, const Vector3& a_vector)
{
    GLuint loc = glGetUniformLocation(m_shaderProgram, a_name);
    //�x�N�g���f�[�^��T����uniform�ɑ���
    glUniform3fv(loc, 1, a_vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char* a_name, float a_value)
{
    GLuint loc = glGetUniformLocation(m_shaderProgram, a_name);
    //float�f�[�^��T����uniform�ɑ���
    glUniform1f(loc, a_value);
}
