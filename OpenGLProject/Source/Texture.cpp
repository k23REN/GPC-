#include "Texture.h"

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SOIL/SOIL.h>

bool Texture::Load(std::string_view fileName) {
  int channels = 0;

  unsigned char* image = SOIL_load_image(fileName.data(), &m_width, &m_height,
                                         &channels, SOIL_LOAD_AUTO);

  if (image == nullptr) {
    SDL_Log("SOIL failed to load texture %s: %s", fileName.data(),
            SOIL_last_result());
    return false;
  }

  int format = GL_RGB;
  if (channels == 4) {
    format = GL_RGBA;
  }

  glGenTextures(1, &m_textureID);
  glBindTexture(GL_TEXTURE_2D, m_textureID);

  glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format,
               GL_UNSIGNED_BYTE, image);

  SOIL_free_image_data(image);

  //バイナリフィルタを有効化
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return true;
}

void Texture::Unload() { glDeleteTextures(1, &m_textureID); }

void Texture::SetActive() { glBindTexture(GL_TEXTURE_2D, m_textureID); }
