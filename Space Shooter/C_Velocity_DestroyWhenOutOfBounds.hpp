#ifndef C_Velocity_DestroyWhenOutOfBounds_hpp
#define C_Velocity_DestroyWhenOutOfBounds_hpp

#include <glm/vec2.hpp>

#include "Component.hpp"

class C_Velocity_DestroyWhenOutOfBounds final : public Component
{
public:
	explicit C_Velocity_DestroyWhenOutOfBounds(Object* initialOwner);

	void update(const float dt) override;

	void set(const glm::vec2& velocity);
	void set(const float x, const float y);
	[[nodiscard]] const glm::vec2& get() const;

private:
	glm::vec2 m_velocity{};
};

#endif
