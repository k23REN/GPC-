//==================================
//実行時のアニメーション設定ボタン
//1キー：歩き
//2キー：パンチ
//3キー：ジャンプ
//==================================
#pragma once
#include "Actor.h"
#include "AnimSpriteComponent.h"
class Human : public Actor
{
public:
	Human(class Game* game);
	void UpdateActor(float deltaTime) override final;
	void ProcessKeyboard(const uint8_t* state);

	float GetRightSpeed() const { return m_rightSpeed; }
	float GetDownSpeed() const { return m_downSpeed; }
private:
	float m_rightSpeed;
	float m_downSpeed;
	AnimSpriteComponent* m_asc;
};