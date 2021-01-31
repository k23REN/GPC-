#pragma once
#include"Actor.h"

class Component
{
public:
	Component(Actor* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void Update(float deltaTime) {};

	virtual void ProcessInput(const uint8_t* keyState) {}

	[[nodiscard]] int GetUpdateOrder() const { return m_updateOrder; }

protected:
	Actor* m_pOwner;

	int m_updateOrder;
};
