#ifndef playerH
#define playerH

#include "Entity.hpp"

namespace Engine
{
	class InputManager;

	class Player : public Entity
	{
		public:
			Player(const glm::vec2&, const glm::vec2&, const glm::vec4&);
			bool update(float, const std::unique_ptr<InputManager>&);
			inline const glm::vec2& getStartVelocity() const { return m_startVelocity; }
			inline const glm::vec2& getStartPosition() const { return m_startPosition; }
			inline int getScore() const { return m_score; }
			inline void setScore(int score) { m_score = score; }
			inline const std::string& getLivesIcon() const { return m_livesIcon; }
			inline void setLivesIcon(const std::string& icon) { m_livesIcon = icon; }
		private:
			std::string m_livesIcon;
			glm::vec2 m_startVelocity;
			glm::vec2 m_startPosition;
			int m_score{ 0 };
	};
}
#endif