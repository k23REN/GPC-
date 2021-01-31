#include "Actor.h"

#include <algorithm>

#include "Component.h"
#include "Game.h"

Actor::Actor(Game* game)
    : m_state(eActive),
      m_position(Vector3::Zero),
      m_scale(1.0f),
      m_rotation(Quaternion::Identity),
      m_pGame(game) {
  m_pGame->AddActor(this);
  m_recomputeWorldTransform = true;
}

Actor::~Actor() { m_pGame->RemoveActor(this); }

void Actor::Update(float deltaTime) {
  if (m_state == eActive || m_state == eInvisible) {
    ComputeWorldTransform();
    UpdateComponents(deltaTime);
    UpdateActor(deltaTime);
    ComputeWorldTransform();
  }
}

void Actor::UpdateComponents(float deltaTime) {
  for (auto comp : m_components) {
    comp->Update(deltaTime);
  }
}

void Actor::UpdateActor(float deltaTime) {}

void Actor::ActorInput(const uint8_t* keyState) {}

void Actor::ProcessInput(const uint8_t* keyState) {
  if (m_state == eActive) {
    for (auto comp : m_components) {
      comp->ProcessInput(keyState);
    }

    ActorInput(keyState);
  }
}

void Actor::AddComponent(Component* component) {
  int myOrder = component->GetUpdateOrder();
  auto iter = m_components.begin();
  for (; iter != m_components.end(); ++iter) {
    if (myOrder < (*iter)->GetUpdateOrder()) {
      break;
    }
  }

  m_components.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
  auto iter = std::find(m_components.begin(), m_components.end(), component);
  if (iter != m_components.end()) {
    m_components.erase(iter);
  }
}

void Actor::ComputeWorldTransform() {
  if (m_recomputeWorldTransform) {
    m_recomputeWorldTransform = false;

    //スケーリング、回転、平行移動の順に計算
    m_worldTransform = Matrix4::CreateScale(m_scale);
    m_worldTransform *= Matrix4::CreateFromQuaternion(m_rotation);
    m_worldTransform *= Matrix4::CreateTranslation(m_position);

    for (auto&& comp : m_components) {
      comp->OnUpdateWorldTransform();
    }
  }
}
