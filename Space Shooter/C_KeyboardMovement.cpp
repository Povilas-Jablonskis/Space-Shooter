#include "C_KeyboardMovement.hpp"
#include "Object.hpp"
#include "BindableAction.hpp"
#include "InputManager.hpp"

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
	const auto moveLeftKey = owner->context->inputManager->getKeybind(BindableAction::MOVE_LEFT);
	const auto moveRightKey = owner->context->inputManager->getKeybind(BindableAction::MOVE_RIGHT);
	const auto moveBackKey = owner->context->inputManager->getKeybind(BindableAction::MOVE_BACK);

	if (inputManager->isKeyActive(moveLeftKey))
	{
		xMove = m_moveSpeed;
	}
	if (inputManager->isKeyActive(moveRightKey))
	{
		xMove = -m_moveSpeed;
	}

	float yMove = m_moveSpeed / 2.0f;
	if (inputManager->isKeyActive(moveBackKey))
	{
		yMove = -m_moveSpeed;
	}

	m_velocity->set(xMove, yMove);
}
