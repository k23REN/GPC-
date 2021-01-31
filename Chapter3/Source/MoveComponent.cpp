#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	: Component(owner, updateOrder)
{
}

void MoveComponent::Update(float deltaTime)
{
	//�p�x�ݒ�
	if (!Math::NearZero(m_angularSpeed)) {	//���ׂȒl�Ƃ̔�r���s��(0���Z�΍�??)
		float rot = m_pOwner->GetRotation();
		rot += m_angularSpeed * deltaTime;
		m_pOwner->SetRotation(rot);
	}

	//�ړ��ݒ�
    if (!Math::NearZero(m_sumOfForces.Length())) {
		Vector2 pos = m_pOwner->GetPosition();
        // �͂̍��v��������x���v�Z
        m_acceleration = m_sumOfForces / m_mass;
        // ���x���X�V
        m_velocity += m_acceleration * deltaTime;

        m_sumOfForces = Vector2::Zero;

		// ��������l�ɂȂ�Ȃ��悤�������s��
        if (m_velocity.x > m_maxSpeed) {
          m_velocity.x = m_maxSpeed;
		}
        if (m_velocity.y > m_maxSpeed) {
          m_velocity.y = m_maxSpeed;
        }

        pos += m_pOwner->GetForward() * m_velocity * deltaTime;

		//�X�N���[���̌��E�ʒu��ݒ�
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
