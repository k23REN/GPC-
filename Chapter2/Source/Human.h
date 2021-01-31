//==================================
//���s���̃A�j���[�V�����ݒ�{�^��
//1�L�[�F����
//2�L�[�F�p���`
//3�L�[�F�W�����v
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