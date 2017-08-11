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

#include "Pickup.h"
#include "UIElement.h"
#include "Player.h"
#include "TestEnemy.h"
#include "Explosion.h"

namespace Engine
{
	class Application
	{
		public:
			~Application();
			Application();
			static std::shared_ptr<Application> instance();
			
			inline GameState getState() const { return gameState; }
			inline void setState(GameState state) { gameState = state; }
			void addEnemyToList(std::shared_ptr<TestEnemy> enemy) { enemies.push_back(enemy); };
			void removeEnemyFromList(std::vector<std::shared_ptr<TestEnemy>>::iterator*);
			void addExplosionToList(std::shared_ptr<Explosion> explosion) { explosions.push_back(explosion); };
			void removeExplosionFromList(std::vector<std::shared_ptr<Explosion>>::iterator*);
			template <class T>
			void timer(const T& callback, unsigned int timeInMs);

			void render();
			void keyboardInputUp(unsigned char, int, int);
			void resize(int, int);
			void keyboardInput(unsigned char, int, int);
			void motionFunc(int, int);
			void processMouseClick(int, int, int, int);
			void specialKeyInput(int, int, int);
			void specialKeyInputUp(int, int, int);
		private:
			void startNewLevel();
			void initPlayerUI();
			void updatePlayerUI();
			void initScene();
			void initGameUI();

			static std::shared_ptr<Application> instance_;
			std::shared_ptr<Player> player;
			std::shared_ptr<UIElement> background;
			std::shared_ptr<UIElement> currentMenu;

			float t;
			float dt;
			float currentTime;
			float accumulator;

			std::vector<std::shared_ptr<Pickup>> pickups;
			std::map<std::string, std::shared_ptr<SpriteSheet>> spriteSheets;
			std::vector<std::shared_ptr<TestEnemy>> enemies;
			std::vector<std::shared_ptr<Explosion>> explosions;
			std::map<std::string, std::shared_ptr<UIElement>> ui;
			std::map<std::string, std::shared_ptr<UIElement>> playerUI;


			std::shared_ptr<CollisionManager> collisionManager;
			std::shared_ptr<Renderer> renderer;
			std::shared_ptr<InputManager> inputManager;
			std::shared_ptr<FontManager> fontManager;
			GameState gameState;
	};
}
#endif