#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder)
	:m_pOwner(owner)
	,m_updateOrder(updateOrder)
{
	m_pOwner->AddComponent(this);
}

Component::~Component()
{
	m_pOwner->RemoveComponent(this);
}
