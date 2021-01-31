#include "CircleComponent.h"

CircleComponent::CircleComponent(Actor* owner)
    : Component(owner), m_radius(0.0f){}

const Vector2& CircleComponent::GetCenter() const {
  return m_pOwner->GetPosition();
}

float CircleComponent::GetRadius() const {
  return m_pOwner->GetScale() * m_radius;
}

bool Intersect(const CircleComponent& a, const CircleComponent& b) {
  // 2‚Â‚Ì‰~‚Ì”¼Œa‚Ì’·‚³
  Vector2 diff = a.GetCenter() - b.GetCenter();
  float distSq = diff.LengthSq();

  // 2‚Â‚Ì‰~‚Ì”¼Œa‚Ì2æ
  float radiiSq = a.GetRadius() + b.GetRadius();
  radiiSq *= radiiSq;

  return distSq <= radiiSq;
}
