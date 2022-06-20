#ifndef entityH
#define entityH

#include "BaseGameObject.hpp"

class Entity : public BaseGameObject
{
public:
	Entity(const glm::vec2&, const glm::vec2&, const glm::vec4&);
	std::vector<std::shared_ptr<BaseGameObject>>* getBulletsList() { return &m_bullets; }
	void addBullet(const std::shared_ptr<BaseGameObject>&);
	bool update(float) override;
	void clearBullets() { m_bullets.clear(); }
	float getDelayBetweenShootsTimer() const { return m_delayBetweenShootsTimer; }
	void setDelayBetweenShootsTimer(const float delayBetweenShootsTimer) { m_delayBetweenShootsTimer = delayBetweenShootsTimer; }
	float getDelayBetweenShoots() const { return m_delayBetweenShoots; }
	void setDelayBetweenShoots(const float delayBetweenShoots) { m_delayBetweenShoots = delayBetweenShoots; }
	std::unordered_map<std::string, std::shared_ptr<BaseGameObject>>* getAddons() { return &m_addons; }
	std::shared_ptr<BaseGameObject> getAddon(const std::string&);
	void addAddon(const std::string&, const std::shared_ptr<BaseGameObject>&);
	std::function<void()> shootingModeFunc;
private:
	std::vector<std::shared_ptr<BaseGameObject>> m_bullets;
	std::unordered_map<std::string, std::shared_ptr<BaseGameObject>> m_addons;
	float m_delayBetweenShootsTimer{};
	float m_delayBetweenShoots{};
};
#endif