#ifndef S_Collidable_hpp
#define S_Collidable_hpp

#include "C_Collider.hpp"
#include "EnumClassHash.hpp"
#include "ComponentPairHash.hpp"
#include "Bitmask.hpp"

#include <vector>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>

class Object;
class C_BoxCollider;

class S_Collidable
{
public:
    S_Collidable();
    void add(std::vector<std::shared_ptr<Object>>&);
    void processRemovals();
    void resolve();

    void update();

private:
    void ProcessCollidingObjects();

    std::unordered_map<CollisionLayer, Bitmask, EnumClassHash> m_collisionLayers;
    std::unordered_map<CollisionLayer, std::vector<std::shared_ptr<C_BoxCollider>>, EnumClassHash> m_collidables;
    std::unordered_set<std::pair<std::shared_ptr<C_BoxCollider>, std::shared_ptr<C_BoxCollider>>, ComponentPairHash> m_objectsColliding;
};



#endif