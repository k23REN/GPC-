#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	, m_pTexture(nullptr)
	, m_drawOrder(drawOrder)
	, m_texWidth(0)
	, m_texHeight(0)
{
	m_pOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	m_pOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (m_pTexture) {
		SDL_Rect r;

		r.w = static_cast<int>(m_texWidth * m_pOwner->GetScale());
		r.h = static_cast<int>(m_texHeight * m_pOwner->GetScale());

		r.x = static_cast<int>(m_pOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(m_pOwner->GetPosition().y - r.h / 2);

		SDL_RenderCopyEx(renderer,
						 m_pTexture,
						 nullptr,
						 &r,
						 -Math::ToDegrees(m_pOwner->GetRotation()),
						 nullptr,
						 SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	m_pTexture = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &m_texWidth, &m_texHeight);
}
