#ifndef playerH
#define playerH

#include "BaseGameObject.hpp"
#include "Entity.hpp"

class InputManager;

class Player final : public Entity
{
public:
	Player(const glm::vec2&, const glm::vec2&, const glm::vec4&);
	using BaseGameObject::update;
	bool update(float, const std::shared_ptr<InputManager>&);
	const glm::vec2& getStartVelocity() const { return m_startVelocity; }
	const glm::vec2& getStartPosition() const { return m_startPosition; }
	const std::string& getLivesIcon() const { return m_livesIcon; }
	void setLivesIcon(const std::string& icon) { m_livesIcon = icon; }
private:
	std::string m_livesIcon{};
	glm::vec2 m_startVelocity{};
	glm::vec2 m_startPosition{};
};
#endif