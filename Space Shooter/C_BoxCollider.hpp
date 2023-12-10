#ifndef C_BoxCollider_hpp
#define C_BoxCollider_hpp

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Component.hpp"
#include "C_Collider.hpp"

class C_BoxCollider final : public C_Collider
{
public:
	explicit C_BoxCollider(Object* initialOwner);

	Manifold intersects(const std::shared_ptr<C_Collider>& other) override;

	void setCollidable(const glm::vec4& AABB);
	const glm::vec4& getCollidable();

	void setOffset(const glm::vec2& offset);
	void setOffset(float offsetX, float offsetY);

	void setSize(const glm::vec2& AABB);
	void setSize(float width, float height);

private:
	void setPosition();

	glm::vec4 m_AABB{};
	glm::vec2 m_offset{};
};

#endif
