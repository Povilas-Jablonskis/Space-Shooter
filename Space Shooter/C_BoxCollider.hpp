#ifndef C_BoxCollider_hpp
#define C_BoxCollider_hpp

#include <math.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Component.hpp"
#include "C_Collider.hpp"

class C_BoxCollider : public C_Collider
{
public:
    C_BoxCollider(Object*);

    Manifold intersects(const std::shared_ptr<C_Collider>&) override;

    void setCollidable(const glm::vec4&);
    const glm::vec4& getCollidable();

    void setOffset(const glm::vec2&);
    void setOffset(float, float);

    void setSize(const glm::vec2&);
    void setSize(float, float);

private:
    void setPosition();

    glm::vec4 m_AABB{};
    glm::vec2 m_offset{};
};

#endif