#ifndef _applicationH
#define _applicationH

#include <iostream>
#include <agents.h>
#include <ppltasks.h>

#include <irrKlang.h>

#include "Renderer.h"
#include "GameState.h"
#include "SpriteSheet.h"
#include "CollisionManager.h"
#include "SpriteSheetManager.h"
#include "MenuManager.h"
#include "Player.h"
#include "Observer.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<Text>> uiPlayerElement;

	class Application : public Observer
	{
		public:
			~Application();
			Application();

			void onNotify(ObserverEvent);
			void onNotify(ObserverEvent, BaseGameObject*);

			inline void setGameState(GameState _state) { lastGameState = getGameState(); gameState = _state; onNotify(ObserverEvent::GAMESTATE_CHANGED); }
			inline GameState getGameState() { return gameState; }
			inline GameState getLastGameState() { return lastGameState; }

			void render();

			void loadPlayerModels();
			void loadPlayerModel();
			void savePlayerConfig();
			void loadPlayerConfig();
			void updatePlayerLives();
			void updatePlayerScore();
			void initLevel();
			void initGameMenu();

			inline irrklang::ISoundEngine* getSoundEngine() { return soundEngine; }
			inline std::shared_ptr<InputManager> getInputManager() { return inputManager; }
			inline std::shared_ptr<MenuManager> getMenuManager() { return menuManager; }
		private:
			irrklang::ISoundEngine* soundEngine;

			std::shared_ptr<Player> player;
			std::shared_ptr<UIElementBase> background;

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

			std::shared_ptr<ConfigurationManager> configurationManager;
			std::shared_ptr<MenuManager> menuManager;
			std::shared_ptr<SpriteSheetManager> spriteSheetManager;
			std::shared_ptr<CollisionManager> collisionManager;
			std::shared_ptr<InputManager> inputManager;

			std::shared_ptr<Renderer> renderer;

			GameState lastGameState;
			GameState gameState;
	};
}
#endif