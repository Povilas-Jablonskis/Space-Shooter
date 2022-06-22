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
    Tile = 3,       // bit 2
    Projectile = 4,
    NPC = 5
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

    virtual Manifold intersects(std::shared_ptr<C_Collider>) = 0;
    virtual void resolveOverlap(const Manifold&) = 0;

    CollisionLayer getLayer() const;
    void setLayer(CollisionLayer);

private:
    CollisionLayer m_layer;
};



#endif