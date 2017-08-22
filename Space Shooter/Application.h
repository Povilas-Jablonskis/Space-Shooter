#ifndef _applicationH
#define _applicationH

#include <map>
#include <memory>
#include <string>

#include <iostream>
#include <agents.h>
#include <ppltasks.h>

#include "Renderer.h"
#include "FontManager.h"
#include "GameState.h"
#include "InputManager.h"
#include "SpriteSheet.h"
#include "CollisionManager.h"
#include "SpriteSheetManager.h"
#include "PickupManager.h"

#include "UIElement.h"
#include "Player.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Observer.h"
#include "Timer.h"

namespace Engine
{
	class Application : public Observer
	{
		public:
			~Application();
			Application();
			std::string virtualKeyCodeToString(SHORT);
			inline GameState getState() const { return gameState; }
			inline void setState(GameState state) { gameState = state; }
			void addEnemyToList(std::shared_ptr<Enemy> enemy) { enemies.push_back(enemy); };
			void removeEnemyFromList(std::vector<std::shared_ptr<Enemy>>::iterator*);
			void addExplosionToList(std::shared_ptr<Explosion> explosion) { explosions.push_back(explosion); };
			void removeExplosionFromList(std::vector<std::shared_ptr<Explosion>>::iterator*);

			void render();
			void keyboardInputUp(unsigned char, int, int);
			void resize(int, int);
			void keyboardInput(unsigned char, int, int);
			void motionFunc(int, int);
			void processMouseClick(int, int, int, int);
			void specialKeyInput(int, int, int);
			void specialKeyInputUp(int, int, int);
		private:
			void initEffects();
			void startNewLevel();
			void initPlayerUI();
			void initSpriteSheets();
			void updatePlayerHealth();
			void updatePlayerScore();
			void initScene();
			void initGameUI();

			std::shared_ptr<Player> player;
			std::shared_ptr<UIElementBase> background;
			std::shared_ptr<UIElement> currentMenu;

			float t;
			float dt;
			float currentTime;
			float accumulator;

			std::map<std::string, std::function<void(Player*)>> effects;
			std::vector<std::shared_ptr<Pickup>> pickups;
			std::vector<std::shared_ptr<Enemy>> enemies;
			std::vector<std::shared_ptr<Explosion>> explosions;
			std::map<std::string, std::shared_ptr<UIElement>> ui;
			std::map<std::string, std::shared_ptr<UIElement>> playerUI;

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