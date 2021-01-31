#pragma once
#include "Component.h"
#include "SDL/SDL.h"
class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);

	const int GetDrawOrder() const { return m_drawOrder; }
	const int GetTexWidth() const { return m_texWidth; }
	const int GetTexHeight() const { return m_texHeight; }

protected:
	SDL_Texture* m_pTexture;
	int m_drawOrder;
	int m_texWidth;
	int m_texHeight;
};
