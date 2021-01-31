#pragma once
#include "Component.h"
#include "SDL/SDL.h"
#include "Shader.h"
#include "Texture.h"
class SpriteComponent : public Component {
 public:
  SpriteComponent(Actor* a_owner, int a_drawOrder = 100);
  ~SpriteComponent();

  virtual void Draw(Sptr<Shader> a_shader);
  virtual void SetTexture(Sptr<Texture> a_texture);

  [[nodiscard]] const int GetDrawOrder() const { return m_drawOrder; }
  [[nodiscard]] const int GetTexWidth() const { return m_texWidth; }
  [[nodiscard]] const int GetTexHeight() const { return m_texHeight; }

  void SetVisible(bool a_visible) { m_visible = a_visible; }
  bool GetVisible() const { return m_visible; }

 protected:
  Sptr<Texture> m_pTexture;
  int m_drawOrder;
  int m_texWidth;
  int m_texHeight;
  bool m_visible;
};
