#pragma once
#include <vector>

#include "Math.h"
#include "Utility.h"
class Actor : public std::enable_shared_from_this<Actor> {
 public:
  /**
   * @enum State
   * 現在の状態(ステータス)
   */
  enum State { eActive, ePaused, eDead, eInvisible };

  Actor(class Game* game);
  virtual ~Actor();

  void Update(float deltaTime);
  void UpdateComponents(float deltaTime);
  virtual void UpdateActor(float deltaTime);

  ///
  ///取得
  ///
  [[nodiscard]] const Vector3& GetPosition() const { return m_position; }
  [[nodiscard]] float GetScale() const { return m_scale; }
  [[nodiscard]] const Quaternion& GetRotation() const { return m_rotation; }
  [[nodiscard]] const State& GetState() const { return m_state; }
  [[nodiscard]] class Game* GetGame() { return m_pGame; }
  [[nodiscard]] const Matrix4& GetWorldTransform() { return m_worldTransform; }
  //! 右向きの軸をクォータニオンで回転
  [[nodiscard]] Vector3 GetRight() const {return Vector3::Transform(Vector3::UnitY, m_rotation);}

  ///
  ///設定
  ///
  void SetPosition(const Vector3& a_pos) {
    m_position = a_pos;
    m_recomputeWorldTransform = true;
  }
  void SetScale(float a_scale) {
    m_scale = a_scale;
    m_recomputeWorldTransform = true;
  }
  void SetRotation(const Quaternion& a_rotation) {
    m_rotation = a_rotation;
    m_recomputeWorldTransform = true;
  }
  void SetState(const State& a_state) { m_state = a_state; }

  //! アクターごとの入力コード
  virtual void ActorInput(const uint8_t* keyState);
  void ProcessInput(const uint8_t* keyState);

  //!前方ベクトル
  Vector3 GetForward() const {
      return Vector3::Transform(Vector3::UnitX, m_rotation);
  }

  void AddComponent(class Component* component);
  void RemoveComponent(class Component* component);

  //!ワールド行列の計算
  void ComputeWorldTransform();

 protected:
  State m_state;

  Vector3 m_position;
  float m_scale;
  Quaternion m_rotation;

  std::vector<class Component*> m_components;
  class Game* m_pGame;

  Matrix4 m_worldTransform;
  bool m_recomputeWorldTransform;
};
