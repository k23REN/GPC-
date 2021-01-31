#pragma once
#include <vector>
#include "Math.h"
#include "Utility.h"
class Actor : public std::enable_shared_from_this<Actor>
{
public:

	/**
	* @enum State
	* 現在の状態(ステータス)
	*/
	enum State
	{
		eActive,
		ePaused,
		eDead, 
		eInvisible
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	///
	///取得
	///
	[[nodiscard]]const Vector2& GetPosition() const { return m_position; }
	[[nodiscard]]float GetScale() const { return m_scale; }
	[[nodiscard]]float GetRotation() const { return m_rotation; }
	[[nodiscard]]const State& GetState() const { return m_state; }
	[[nodiscard]]class Game* GetGame() { return m_pGame; }

	///
	///設定
	///
	void SetPosition(const Vector2& pos) { m_position = pos; }
	void SetScale(float scale) { m_scale = scale; }
	void SetRotation(float rotation) { m_rotation = rotation; }
	void SetState(State state) { m_state = state; }

	//! アクターごとの入力コード
	virtual void ActorInput(const uint8_t* keyState);
    void ProcessInput(const uint8_t* keyState);

	//!前方ベクトル
	Vector2 GetForward() const { return Vector2(Math::Cos(m_rotation), -Math::Sin(m_rotation)); }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

 protected:
	State m_state;

	Vector2 m_position;
	float m_scale;
	float m_rotation;

	std::vector<class Component*> m_components;
	class Game* m_pGame;
};
