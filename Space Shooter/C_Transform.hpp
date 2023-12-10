#ifndef C_Transform_hpp
#define C_Transform_hpp

#include <glm/vec2.hpp>

#include "Component.hpp"

class C_Transform final : public Component
{
public:
	explicit C_Transform(Object*);

	void setPosition(float positionX, float positionY);
	void setPosition(const glm::vec2& position);

	void addPosition(const float positionX, const float positionY);
	void addPosition(const glm::vec2 position);

	void setX(const float positionX);
	void setY(const float positionY);

	void addX(const float positionX);
	void addY(const float positionY);

	[[nodiscard]] glm::vec2 getPosition() const;

private:
	glm::vec2 m_position{};
};

#endif
