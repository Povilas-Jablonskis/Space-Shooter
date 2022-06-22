#ifndef C_KeyboardMovement_hpp
#define C_KeyboardMovement_hpp

#include "Component.hpp"

#include <memory>

class C_Velocity;

class C_KeyboardMovement : public Component
{
public:
    C_KeyboardMovement(Object*);

    void awake() override;

    void setMovementSpeed(float);

    void update(float) override;

private:
    float m_moveSpeed{ 100.0f };

    std::shared_ptr<C_Velocity> m_velocity;
};

#endif