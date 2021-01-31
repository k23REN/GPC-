#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, m_currFrame(0.0f)
	, m_animFPS(15.0f)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	auto& anim = m_animMap[m_playKey];

	auto& textures = anim.m_anim_textures;
	if (textures.size() > 0) {
		m_currFrame += m_animFPS * deltaTime;

		while (m_currFrame >= textures.size()) {
			if (anim.m_isLoop) {
				m_currFrame -= textures.size();
			}
			else {
				return;
			}
		}

		SetTexture(textures[static_cast<int>(m_currFrame)]);
	}
}

void AnimSpriteComponent::SetAnim_textures(const std::string& key, const AnimInfo& animInfo)
{
	m_animMap[key] = animInfo;

	auto& animtex = m_animMap[key].m_anim_textures;

	if (animtex.size() > 0) {
		m_currFrame = 0.0f;
		SetTexture(animtex[0]);
	}
}

void AnimSpriteComponent::SetPlayAnim(const std::string& key)
{
	m_playKey = key;

	auto& animtex = m_animMap[key].m_anim_textures;

	if (animtex.size() > 0) {
		m_currFrame = 0.0f;
		SetTexture(animtex[0]);
	}
}
