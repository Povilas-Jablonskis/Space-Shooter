#ifndef baseGameObjectH
#define baseGameObjectH

#include <vector>
#include <memory>

#include "RenderObject.hpp"

class BaseGameObject : public RenderObject
{
public:
	BaseGameObject(const glm::vec2&, const glm::vec2&, const glm::vec4&);
	std::function<void()> onUpdateFunc;
	virtual bool update(float);
	std::function<void(const std::shared_ptr<BaseGameObject>&)> onCollisionFunc;
	void addAnimation(const std::string&, const std::shared_ptr<Animation>&);
	std::shared_ptr<Animation> getAnimationByIndex(const std::string&);
	void setLives(const int lives) { m_lives = lives; }
	int getLives() const { return m_lives; }
	void setVelocity(const glm::vec2& velocity) { m_velocity = velocity; };
	void setVelocity(const int index, const float velocity) { m_velocity[index] = velocity; };
	const glm::vec2& getVelocity() const { return m_velocity; };
	void setNeedsToBeRemoved(const bool boolean) { m_needsToBeRemoved = boolean; };
	bool getNeedsToBeRemoved() const { return m_needsToBeRemoved; }
	int getValue() const { return m_value; }
	void setValue(const int value) { m_value = value; }
	const std::string& getExplosionSound() const { return m_explosionSound; }
	void setExplosionSound(const std::string& explosionSound) { m_explosionSound = explosionSound; }
private:
	std::vector<animation>* getAnimations() { return &m_animations; }

	std::string m_explosionSound;
	bool m_needsToBeRemoved{ false };
	std::vector<animation> m_animations;
	glm::vec2 m_velocity;
	int m_value{ 0 };
	int m_lives{ 1 };
};
#endif