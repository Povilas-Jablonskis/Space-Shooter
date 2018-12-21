#ifndef _applicationH
#define _applicationH

#include <iostream>
#include <agents.h>
#include <ppltasks.h>

#include <irrKlang.h>

#include "Renderer.h"
#include "FontManager.h"
#include "GameState.h"
#include "InputManager.h"
#include "SpriteSheet.h"
#include "CollisionManager.h"
#include "SpriteSheetManager.h"
#include "MenuManager.h"

#include "UIElementBase.h"
#include "Player.h"
#include "Observer.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<UIElementBase>> uiPlayerElement;

	class Application : public Observer
	{
		public:
			~Application();
			Application();
			std::string virtualKeyCodeToString(SHORT);

			inline void setGameState(GameState _state) { lastGameState = getGameState(); gameState = _state; onNotify(ObserverEvent::GAMESTATE_CHANGED, std::vector<std::pair<std::string, BaseGameObject*>>()); }
			inline GameState getGameState() { return gameState; }
			inline GameState getLastGameState() { return lastGameState; }
			void render();
			void keyboardInput(unsigned char, glm::vec2);
			void keyboardInputUp(unsigned char, glm::vec2);
			void resize(int, int);
			void motionFunc(glm::vec2);
			void processMouseClick(int, int, glm::vec2);
			void specialKeyInput(int, glm::vec2);
			void specialKeyInputUp(int, glm::vec2);

			void loadPlayerModels();
			void loadPlayerModel();
			void saveConfig();
			void loadConfig();
			void updatePlayerLives();
			void updatePlayerScore();
			void initScene();
			void initGameUI();
		private:
			irrklang::ISoundEngine* soundEngine;

			std::shared_ptr<Player> player;
			std::shared_ptr<BaseGameObject> background;

			float dt;
			float currentTime;
			float accumulator;

			int currentLevel;

			int characterSelectionIndex;
			std::vector<std::shared_ptr<Animation>> playerModels;

			std::vector<std::shared_ptr<BaseGameObject>> meteors;
			std::vector<std::shared_ptr<BaseGameObject>> pickups;
			std::vector<std::shared_ptr<Entity>> enemies;
			std::vector<std::shared_ptr<BaseGameObject>> explosions;

			std::vector<std::shared_ptr<UIElementBase>> playerLives;
			std::vector<std::shared_ptr<UIElementBase>> scoreBoard;
			std::vector<uiPlayerElement> notifications;

			std::shared_ptr<MenuManager> menuManager;
			std::shared_ptr<SpriteSheetManager> spriteSheetManager;
			std::shared_ptr<CollisionManager> collisionManager;
			std::shared_ptr<Renderer> renderer;
			std::shared_ptr<InputManager> inputManager;
			std::shared_ptr<FontManager> fontManager;
			GameState lastGameState;
			GameState gameState;
	};
}
#endif