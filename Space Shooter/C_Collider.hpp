#ifndef C_Collider_hpp
#define C_Collider_hpp

#include <memory>
#include <glm/vec4.hpp>

#include "Component.hpp"

// enum ints used when defining collision layers
enum class CollisionLayer
{
    Default = 1,    // bit 0
    Player = 2,     // bit 1
    Projectile = 3,
    Enemy = 4,
    Meteor = 5
};

struct Manifold
{
    bool colliding = false;
    const glm::vec4* other;
};

class C_Collider : public Component
{
public:
    C_Collider(Object*);

    virtual Manifold intersects(const std::shared_ptr<C_Collider>&) = 0;

    CollisionLayer getLayer() const;
    void setLayer(CollisionLayer);

private:
    CollisionLayer m_layer;
};



#endif