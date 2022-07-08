#ifndef C_Velocity_ReverseVelocityWhenOutOfBounds_hpp
#define C_Velocity_ReverseVelocityWhenOutOfBounds_hpp

#include <math.h>
#include <glm/vec2.hpp>

#include "Component.hpp"

class C_Velocity_ReverseVelocityWhenOutOfBounds : public Component
{
public:
    C_Velocity_ReverseVelocityWhenOutOfBounds(Object*);

    void update(float) override;

    void set(const glm::vec2&);
    void set(float x, float y);
    const glm::vec2& get() const;
private:
    glm::vec2 m_velocity{};
};

#endif