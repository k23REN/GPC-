#pragma once
#include <vector>
#include "Math.h"
class Actor
{
public:
	enum State
	{
		eActive,
		ePaused,
		eDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	const Vector2& GetPosition() const { return m_position; }
	float GetScale() const { return m_scale; }
	float GetRotation() const { return m_rotation; }
	State GetState() const { return m_state; }
	class Game* GetGame() { return m_pGame; }

	void SetPosition(const Vector2& pos) { m_position = pos; }
	void SetScale(float scale) { m_scale = scale; }
	void SetRotation(float rotation) { m_rotation = rotation; }
	void SetState(State state) { m_state = state; }



	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	State m_state;

	Vector2 m_position;
	float m_scale;
	float m_rotation;

	std::vector<class Component*> m_components;
	class Game* m_pGame;
};
