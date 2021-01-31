#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <unordered_map>
struct AnimInfo
{
	std::vector<SDL_Texture*> m_animTextures;
	bool m_isLoop;
};

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(Actor* owner, int drawOrder = 100);
	void Update(float deltaTime) override final;

	[[nodiscard]] float GetAnimFPS() const { return m_animFPS; }

	void SetAnimTextures(const std::string& key, const AnimInfo& animInfo);
	void SetAnimFPS(float fps) { m_animFPS = fps; }

	//!再生するアニメーション設定
	void SetPlayAnim(const std::string& key);
private:
	std::unordered_map<std::string, AnimInfo> m_animMap;

	std::string m_playKey;
	float m_currFrame;
	float m_animFPS;
};
