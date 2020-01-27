#ifndef levelH
#define levelH

#include <rapidxml/rapidxml.hpp>
#include <string>
#include <irrKlang/irrKlang.h>
#include <vector>
#include <memory>

#include "UIManager.hpp"

namespace Engine
{
	class UIElementBase;
	class Text;
	class BaseGameObject;
	class InputManager;
	class GameStateManager;
	class ConfigurationManager;
	class SpriteSheetManager;
	class CollisionManager;
	class Player;
	class Entity;
	class Renderer;

	typedef std::pair<std::string, std::shared_ptr<Text>> uiPlayerElement;

	class Level
	{
		public:
			Level(rapidxml::xml_node<char>*, const std::unique_ptr<SpriteSheetManager>&, irrklang::ISoundEngine*, int);

			bool update(float, const std::shared_ptr<Player>&, const std::unique_ptr<GameStateManager>&, const std::unique_ptr<InputManager>&, const std::unique_ptr<CollisionManager>&);
			void render(float, const std::shared_ptr<Player>&, const std::unique_ptr<GameStateManager>&, const std::unique_ptr<InputManager>&, const std::unique_ptr<CollisionManager>&, const std::unique_ptr<Renderer>&, const std::unique_ptr<ConfigurationManager>&, const std::unique_ptr<SpriteSheetManager>&);
		
			inline void addExplosion(const std::shared_ptr<BaseGameObject>& _explosion) { m_explosions.push_back(_explosion); }
		private:
			inline const std::unique_ptr<UIManager>& getUIManager() const { return m_uiManager; }

			std::unique_ptr<UIManager> m_uiManager{ std::make_unique<UIManager>() };

			std::vector<std::shared_ptr<BaseGameObject>> m_meteors;
			std::vector<std::shared_ptr<BaseGameObject>> m_pickups;
			std::vector<std::shared_ptr<Entity>> m_enemies;
			std::vector<std::shared_ptr<BaseGameObject>> m_explosions;
			std::shared_ptr<UIElementBase> m_background;
	};
}
#endif