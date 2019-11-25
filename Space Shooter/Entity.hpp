#ifndef entityH
#define entityH

#include "BaseGameObject.hpp"

namespace Engine
{
	class InputComponent;
	typedef std::pair<std::string, std::shared_ptr<BaseGameObject>> addon;

	class Entity : public BaseGameObject
	{
		public:
			Entity(const glm::vec2&, const glm::vec2&, const glm::vec4&);
			inline std::vector<std::shared_ptr<BaseGameObject>>* getBulletsList() { return &m_bullets; }
			void addBullet(const std::shared_ptr<BaseGameObject>&);
			virtual bool update(float);
			inline void clearBullets() { m_bullets.clear(); }
			inline float getDelayBetweenShootsTimer() const { return m_delayBetweenShootsTimer; }
			inline void setDelayBetweenShootsTimer(float delayBetweenShootsTimer) { m_delayBetweenShootsTimer = delayBetweenShootsTimer; }
			inline float getDelayBetweenShoots() const { return m_delayBetweenShoots; }
			inline void setDelayBetweenShoots(float delayBetweenShoots) { m_delayBetweenShoots = delayBetweenShoots; }
			inline std::vector<addon>* getAddons() { return &m_addons; }
			std::shared_ptr<BaseGameObject> getAddon(const std::string&);
			void addAddon(addon);
			std::function<void()> shootingModeFunc;
			inline const std::shared_ptr<InputComponent>& getInputComponent() const { return m_inputComponent; }
		private:
			std::vector<std::shared_ptr<BaseGameObject>> m_bullets;
			std::vector<addon> m_addons;
			float m_delayBetweenShootsTimer{ 0.0f };
			float m_delayBetweenShoots{ 0.0f };
			std::shared_ptr<InputComponent> m_inputComponent{ std::make_shared<InputComponent>() };
	};
}
#endif