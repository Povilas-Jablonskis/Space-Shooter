#ifndef C_Transform_hpp
#define C_Transform_hpp

#include <glm/vec2.hpp>

#include "Component.hpp"

class C_Transform : public Component
{
public:
    C_Transform(Object*);

    void setPosition(float, float);
    void setPosition(const glm::vec2&);

    void addPosition(float, float);
    void addPosition(glm::vec2);

    void setX(float);
    void setY(float);

    void addX(float);
    void addY(float);

    glm::vec2 getPosition() const;
private:
    glm::vec2 m_position{};
};

#endif