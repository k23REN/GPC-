#include "Random.h"

void Random::Init() 
{
  std::random_device rd;
  Random::Seed(rd());
}

void Random::Seed(unsigned int a_seed) 
{ 
    sGenerator.seed(a_seed);
}

float Random::GetFloat() 
{ 
    return GetFloatRange(0.0f, 1.0f);
}

float Random::GetFloatRange(float a_min, float a_max) 
{
  std::uniform_real_distribution<float> dist(a_min, a_max);
  return dist(sGenerator);
}

int Random::GetIntRange(int a_min, int a_max) {
  std::uniform_int_distribution<int> dist(a_min, a_max);
  return dist(sGenerator);
}

Vector2 Random::GetVector(const Vector2& a_min, const Vector2& a_max) {
  Vector2 r = Vector2(GetFloat(), GetFloat());
  return a_min + (a_max - a_min) * r;
}

Vector3 Random::GetVector(const Vector3& a_min, const Vector3& a_max) {
  Vector3 r = Vector3(GetFloat(), GetFloat(), GetFloat());
  return a_min + (a_max - a_min) * r;
}

std::mt19937 Random::sGenerator;
