#ifndef C_KeyboardMovement_hpp
#define C_KeyboardMovement_hpp

#include "Component.hpp"

#include <memory>

#include "C_Velocity_ReverseVelocityWhenOutOfBounds.hpp"

class C_KeyboardMovement final : public Component
{
public:
	explicit C_KeyboardMovement(Object* initialOwner);

	void awake() override;

	void setMovementSpeed(float moveSpeed);

	void update(float dt) override;

private:
	float m_moveSpeed{100.0f};

	std::shared_ptr<C_Velocity_ReverseVelocityWhenOutOfBounds> m_velocity;
};

#endif
