#include "InputComponent.h"

InputComponent::InputComponent(class Actor* owner)
    : MoveComponent(owner) {}

void InputComponent::ProcessInput(const uint8_t* keyState) {
    // �O�i���x
    {
        Vector2 force = Vector2::Zero;
        if (keyState[m_forwardKey]) {
          force = Vector2(50.0f, 50.0f);
        }
        if (keyState[m_backKey]) {
          force = Vector2(-10.0f, -10.0f);
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
