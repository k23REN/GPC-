#pragma once
#include "Component.h"
#include "SDL/SDL.h"
class SpriteComponent : public Component
{
public:
	SpriteComponent(Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);

	[[nodiscard]]const int GetDrawOrder() const { return m_drawOrder; }
	[[nodiscard]]const int GetTexWidth() const { return m_texWidth; }
	[[nodiscard]]const int GetTexHeight() const { return m_texHeight; }

protected:
	SDL_Texture* m_pTexture;
	int m_drawOrder;
	int m_texWidth;
	int m_texHeight;
};
