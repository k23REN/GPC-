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
		Quaternion rot = m_pOwner->GetRotation();
		float angle = m_angularSpeed * deltaTime;
        Quaternion inc(Vector3::UnitZ, angle);
        rot = Quaternion::Concatenate(rot, inc);
        m_pOwner->SetRotation(rot);
	}

	//�ړ��ݒ�
    if (!Math::NearZero(m_forwardSpeed) || !Math::NearZero(m_strafeSpeed)) {
      Vector3 pos = m_pOwner->GetPosition();
      pos += m_pOwner->GetForward() * m_forwardSpeed * deltaTime;
      pos += m_pOwner->GetRight() * m_strafeSpeed * deltaTime;
      m_pOwner->SetPosition(pos);
    }
    /*if (!Math::NearZero(m_sumOfForces.Length())) {
		Vector3 pos = m_pOwner->GetPosition();
        // �͂̍��v��������x���v�Z
        m_acceleration = m_sumOfForces / m_mass;
        // ���x���X�V
        m_velocity += m_acceleration * deltaTime;

        m_sumOfForces = Vector3::Zero;

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
          if (pos.x < -512.0f) {
            pos.x = 510.0f;
          } else if (pos.x > 512.0f) {
            pos.x = -510.0f;
          }
          if (pos.y < -384.0f) {
            pos.y = 382.0f;
          } else if (pos.y > 384.0f) {
            pos.y = -382.0f;
          }
		}

		m_pOwner->SetPosition(pos);
	}*/
}

void MoveComponent::AddForce(const Vector3& a_force) 
{
  m_sumOfForces = a_force;
}
