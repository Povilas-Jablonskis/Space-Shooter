#ifndef baseGameObjectH
#define baseGameObjectH

#include <vector>
#include <memory>

#include "RenderObject.hpp"

namespace Engine
{
	class BaseGameObject : public RenderObject
	{
		public:
			BaseGameObject(const glm::vec2&, const glm::vec2&, const glm::vec4&);
			std::function<void()> onUpdateFunc;
			virtual bool update(float);
			std::function<void(const std::shared_ptr<BaseGameObject>&)> onCollisionFunc;
			void addAnimation(const std::string&, const std::shared_ptr<Animation>&);
			std::shared_ptr<Animation> getAnimationByIndex(const std::string&);
			virtual inline void setLives(int lives) { m_lives = lives; }
			inline int getLives() const { return m_lives; }
			inline void setVelocity(const glm::vec2& velocity) { m_velocity = velocity; };
			inline void setVelocity(int index, float velocity) { m_velocity[index] = velocity; };
			inline const glm::vec2& getVelocity() const { return m_velocity; };
			inline void setNeedsToBeRemoved(bool boolean) { m_needsToBeRemoved = boolean; };
			inline bool getNeedsToBeRemoved() const { return m_needsToBeRemoved; }
			inline int getValue() const { return m_value; }
			inline void setValue(int value) { m_value = value; }
			inline const std::string& getExplosionSound() const { return m_explosionSound; }
			inline void setExplosionSound(const std::string& explosionSound) { m_explosionSound = explosionSound; }
		private:
			inline std::vector<animation>* getAnimations() { return &m_animations; }

			std::string m_explosionSound;
			bool m_needsToBeRemoved{ false };
			std::vector<animation> m_animations;
			glm::vec2 m_velocity;
			int m_value{ 0 };
			int m_lives{ 1 };
	};
}
#endif