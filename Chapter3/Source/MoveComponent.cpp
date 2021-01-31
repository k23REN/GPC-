#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	: Component(owner, updateOrder)
{
}

void MoveComponent::Update(float deltaTime)
{
	//角度設定
	if (!Math::NearZero(m_angularSpeed)) {	//微細な値との比較を行う(0除算対策??)
		float rot = m_pOwner->GetRotation();
		rot += m_angularSpeed * deltaTime;
		m_pOwner->SetRotation(rot);
	}

	//移動設定
    if (!Math::NearZero(m_sumOfForces.Length())) {
		Vector2 pos = m_pOwner->GetPosition();
        // 力の合計から加速度を計算
        m_acceleration = m_sumOfForces / m_mass;
        // 速度を更新
        m_velocity += m_acceleration * deltaTime;

        m_sumOfForces = Vector2::Zero;

		// 速すぎる値にならないよう調整を行う
        if (m_velocity.x > m_maxSpeed) {
          m_velocity.x = m_maxSpeed;
		}
        if (m_velocity.y > m_maxSpeed) {
          m_velocity.y = m_maxSpeed;
        }

        pos += m_pOwner->GetForward() * m_velocity * deltaTime;

		//スクリーンの限界位置を設定
		{
			if (pos.x < 0.0f) { pos.x = 1022.0f; }
			else if (pos.x > 1024.0f) { pos.x = 2.0f; }

			if (pos.y < 0.0f) { pos.y = 766.0f; }
			else if (pos.y > 768.0f) { pos.y = 2.0f; }
		}

		m_pOwner->SetPosition(pos);
	}
}

void MoveComponent::AddForce(const Vector2& a_force) 
{
  m_sumOfForces = a_force;
}
