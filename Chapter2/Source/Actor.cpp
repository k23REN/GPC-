#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:m_state(eActive)
	, m_position(Vector2::Zero)
	, m_scale(1.0f)
	, m_rotation(0.0f)
	, m_pGame(game)
{
	m_pGame->AddActor(this);
}

Actor::~Actor()
{
	m_pGame->RemoveActor(this);
	while (!m_components.empty()) {
		delete m_components.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (m_state == eActive) {
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : m_components) {
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::AddComponent(Component* component)
{
	int myOrder = component->GetUpdateOrder();
	auto iter = m_components.begin();
	for (;
		 iter != m_components.end();
		 ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}

	m_components.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(m_components.begin(), m_components.end(), component);
	if (iter != m_components.end()) {
		m_components.erase(iter);
	}
}
