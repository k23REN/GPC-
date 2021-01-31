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
}

void Actor::Update(float deltaTime)
{
	if (m_state == eActive || m_state == eInvisible) {
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

void Actor::ActorInput(const uint8_t* keyState) {}

void Actor::ProcessInput(const uint8_t* keyState) 
{
  if (m_state == eActive) {
    for (auto comp : m_components) {
      comp->ProcessInput(keyState);
    }

    ActorInput(keyState);
  }
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
