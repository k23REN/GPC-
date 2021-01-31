#pragma once
class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void Update(float deltaTime) {};

	int GetUpdateOrder() const { return m_updateOrder; }
protected:
	class Actor* m_pOwner;

	int m_updateOrder;
};
