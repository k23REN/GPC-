#pragma once
#include "Component.h"
#include "Math.h"

class CircleComponent : public Component 
{
 public:
  CircleComponent(Actor* owner);

  void SetRadius(const float radius) { m_radius = radius; }
  float GetRadius() const;

  const Vector2& GetCenter() const;

 private:
  float m_radius = 0.0f;
};

bool Intersect(const CircleComponent& a, const CircleComponent& b);