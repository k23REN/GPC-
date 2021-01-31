#pragma once

#include<string>

class Texture
{
 public:
  
  bool Load(std::string_view a_fileName);
  void Unload();

  void SetActive();

  [[nodiscard]] const int GetWidth() const { return m_width; }
  [[nodiscard]] const int GetHeight() const { return m_height; }

 private: 
  unsigned int m_textureID = 0;
  int m_width = 0;
  int m_height = 0;
};