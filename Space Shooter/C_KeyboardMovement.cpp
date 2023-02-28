#include "C_KeyboardMovement.hpp"
#include "C_Transform.hpp"
#include "Object.hpp"
#include "InputManager.hpp"
#include "BindableActions.hpp"
#include "C_Velocity_ReverseVelocityWhenOutOfBounds.hpp"

C_KeyboardMovement::C_KeyboardMovement(Object* initialOwner) : Component(initialOwner)
{
}

void C_KeyboardMovement::awake()
{
	m_velocity = owner->getComponent<C_Velocity_ReverseVelocityWhenOutOfBounds>();
}

void C_KeyboardMovement::setMovementSpeed(const float moveSpeed)
{
	m_moveSpeed = moveSpeed;
}

void C_KeyboardMovement::update(float)
{
	float xMove = 0.f;
	const auto inputManager = owner->context->inputManager;

	if (inputManager->isKeyActive(BindableActions::MOVE_LEFT))
	{
		xMove = -m_moveSpeed;
	}
	if (inputManager->isKeyActive(BindableActions::MOVE_RIGHT))
	{
		xMove = m_moveSpeed;
	}

	float yMove = m_moveSpeed / 2.0f;
	if (inputManager->isKeyActive(BindableActions::MOVE_BACK))
	{
		yMove = -m_moveSpeed;
	}

	m_velocity->set(xMove, yMove);
}
