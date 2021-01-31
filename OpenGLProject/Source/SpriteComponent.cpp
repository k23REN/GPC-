#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "GL/glew.h"

SpriteComponent::SpriteComponent(Actor* a_owner, int a_drawOrder)
	:Component(a_owner)
	, m_pTexture(nullptr)
	, m_drawOrder(a_drawOrder)
	, m_texWidth(0)
	, m_texHeight(0)
{
  m_pOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
  m_pOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Sptr<Shader> a_shader) {
  if (m_pOwner->GetState() == Actor::State::eInvisible) return;

  if (m_pTexture && m_visible) {
    //テクスチャの幅高さでスケーリング
    Matrix4 scaleMat = Matrix4::CreateScale(
        static_cast<float>(m_texWidth), static_cast<float>(m_texHeight), 1.0f);

    Matrix4 world = scaleMat * m_pOwner->GetWorldTransform();

    a_shader->SetMatrixUniform("uWorldTransform", world);

    m_pTexture->SetActive();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  }
}

void SpriteComponent::SetTexture(Sptr<Texture> a_texture) {
  m_pTexture = a_texture;
  m_texWidth = a_texture->GetWidth();
  m_texHeight = a_texture->GetHeight();
}
