#include "C_KeyboardMovement.hpp"
#include "C_Velocity.hpp"
#include "Object.hpp"
#include "InputManager.hpp"
#include "BindableActions.hpp"

C_KeyboardMovement::C_KeyboardMovement(Object* owner) : Component(owner) {}

void C_KeyboardMovement::awake()
{
    m_velocity = m_owner->getComponent<C_Velocity>();
}

void C_KeyboardMovement::setMovementSpeed(float moveSpeed)
{
    m_moveSpeed = moveSpeed;
}

void C_KeyboardMovement::update(float deltaTime)
{
    float xMove = 0.f;
    if (m_owner->m_context->m_inputManager->getKey(BindableActions::MOVE_LEFT))
    {
        xMove = -m_moveSpeed;
    }
    if (m_owner->m_context->m_inputManager->getKey(BindableActions::MOVE_RIGHT))
    {
        xMove = m_moveSpeed;
    }

    float yMove = m_moveSpeed / 2.0f;
    if (m_owner->m_context->m_inputManager->getKey(BindableActions::MOVE_BACK))
    {
        yMove = -m_moveSpeed;
    }

    m_velocity->set(xMove, yMove);
}