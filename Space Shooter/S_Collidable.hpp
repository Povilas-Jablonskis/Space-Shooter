#ifndef S_Collidable_hpp
#define S_Collidable_hpp

#include "C_Collider.hpp"
#include "EnumClassHash.hpp"
#include "ComponentPairHash.hpp"
#include "Bitmask.hpp"
#include "C_BoxCollider.hpp"
#include "Object.hpp"

#include <vector>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>

class S_Collidable
{
public:
	S_Collidable();
	void add(const std::vector<std::shared_ptr<Object>>& objects);
	void processRemovals();
	void resolve();
	void processCollidingObjects();

private:
	std::unordered_map<CollisionLayer, Bitmask, EnumClassHash> m_collisionLayers;
	std::unordered_map<CollisionLayer, std::vector<std::shared_ptr<C_BoxCollider>>, EnumClassHash> m_collidables;
	std::unordered_set<std::pair<std::shared_ptr<C_BoxCollider>, std::shared_ptr<C_BoxCollider>>, ComponentPairHash>
	m_objectsColliding;
};


#endif
