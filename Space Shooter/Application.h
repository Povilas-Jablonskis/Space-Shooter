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
#include "PickupManager.h"
#include "EnemyManager.h"
#include "EffectManager.h"

#include "UIElementBase.h"
#include "Player.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Observer.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<UIElementBase>> uiPlayerElement;
	typedef std::pair<std::string, std::vector<std::shared_ptr<UIElementBase>>> menu;

	class Application : public Observer
	{
		public:
			~Application();
			Application();
			std::string virtualKeyCodeToString(SHORT);
			inline GameState getState() const { return gameState; }
			inline void setState(GameState state) { gameState = state; }
			void removeEnemyFromList(std::vector<std::shared_ptr<Enemy>>::iterator*);
			void addExplosionToList(std::shared_ptr<Explosion> explosion) { explosions.push_back(explosion); };
			inline std::shared_ptr<FontManager> getFontManager() { return fontManager; };
			inline std::shared_ptr<InputManager> getInputManager() { return inputManager; };
			std::shared_ptr<UIElementBase> getPlayerUIElement(std::string);
			void erasePlayerUIElement(std::string);
			std::shared_ptr<menu> getMenu(std::string);

			void render();
			void keyboardInputUp(unsigned char, int, int);
			void resize(int, int);
			void keyboardInput(unsigned char, int, int);
			void motionFunc(int, int);
			void processMouseClick(int, int, int, int);
			void specialKeyInput(int, int, int);
			void specialKeyInputUp(int, int, int);

			void saveConfig();
			void loadConfig();
			void startNewLevel();
			void updatePlayerHealth();
			void updatePlayerScore();
			void resetScene();
			void initScene();
			void initGameUI();
		private:
			irrklang::ISoundEngine* soundEngine;

			std::shared_ptr<Player> player;
			std::shared_ptr<UIElementBase> background;

			float t;
			float dt;
			float currentTime;
			float accumulator;

			int currentLevel;

			std::vector<std::shared_ptr<BaseGameObject>> meteors;
			std::vector<std::shared_ptr<BaseGameObject>> pickups;
			std::vector<std::shared_ptr<Enemy>> enemies;
			std::vector<std::shared_ptr<Explosion>> explosions;
			std::vector<uiPlayerElement> playerUI;
			std::vector<std::shared_ptr<menu>> gameMenu;
			std::shared_ptr<menu> currentMenu;

			std::shared_ptr<EffectManager> effectManager;
			std::shared_ptr<EnemyManager> enemyManager;
			std::shared_ptr<PickupManager> pickupManager;
			std::shared_ptr<SpriteSheetManager> spriteSheetManager;
			std::shared_ptr<CollisionManager> collisionManager;
			std::shared_ptr<Renderer> renderer;
			std::shared_ptr<InputManager> inputManager;
			std::shared_ptr<FontManager> fontManager;
			GameState gameState;
	};
}
#endif