#include "InputComponent.h"

InputComponent::InputComponent(Actor* owner)
    : MoveComponent(owner) {}

void InputComponent::ProcessInput(const uint8_t* keyState) {
    // �O�i���x
    {
        Vector3 force = Vector3::Zero;
        if (keyState[m_forwardKey]) {
          force = Vector3(0.0f, 0.0f, 50.0f);
        }
        if (keyState[m_backKey]) {
          force = Vector3(0.0f, 0.0f, -50.0f);
        }

        AddForce(force);
    }

    // ��]���x
    {
        float angularSpeed = 0.0f;
        if (keyState[m_clockwiseKey]) {
          angularSpeed += m_maxAngularSpeed;
        }
        if (keyState[m_counterClockwiseKey]) {
          angularSpeed -= m_maxAngularSpeed;
        }
        SetAngularSpeed(angularSpeed);
    }
}
