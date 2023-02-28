#ifndef C_Collider_hpp
#define C_Collider_hpp

#include <memory>
#include <glm/vec4.hpp>

#include "Component.hpp"

// enum integers used when defining collision layers
enum class CollisionLayer
{
	DEFAULT = 1,
	// bit 0
	PLAYER = 2,
	// bit 1
	PROJECTILE = 3,
	ENEMY = 4,
	METEOR = 5
};

struct Manifold
{
	bool colliding = false;
	const glm::vec4* other{};
};

class C_Collider : public Component
{
public:
	explicit C_Collider(Object*);

	virtual Manifold intersects(const std::shared_ptr<C_Collider>&) = 0;

	[[nodiscard]] CollisionLayer getLayer() const;
	void setLayer(CollisionLayer);

private:
	CollisionLayer m_layer;
};


#endif
