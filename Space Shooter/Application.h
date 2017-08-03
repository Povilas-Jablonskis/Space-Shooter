#ifndef _applicationH
#define _applicationH

#include <unordered_map>
#include <memory>
#include <string>

#include <iostream>
#include <agents.h>
#include <ppltasks.h>

#include "document.h"
#include "filereadstream.h"
#include "filewritestream.h"
#include "writer.h"

#include "Renderer.h"
#include "FontManager.h"
#include "GameState.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "CollisionManager.h"

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
			inline std::shared_ptr<Font> getFont(const std::string& name) { return fontManager->getFont(name); }
			void checkCollision(std::shared_ptr<BaseGameObject>, std::vector<std::shared_ptr<BaseGameObject>>*);
			void checkCollision(std::shared_ptr<BaseGameObject>, std::vector<std::shared_ptr<Bullet>>*, std::shared_ptr<BaseGameObject>);
			inline GameState getState() const { return gameState; }
			inline void setState(GameState state) { gameState = state; }

			template <class T>
			void timer(const T& callback, unsigned int timeInMs);

			void startNewLevel();
			void initPlayerUI();
			void updatePlayerUI();
			void initScene();
			void initGameUI();
			void render();
			void keyboardInputUp(unsigned char, int, int);
			void resize(int, int);
			void keyboardInput(unsigned char, int, int);
			void motionFunc(int, int);
			void processMouseClick(int, int, int, int);
		private:
			std::shared_ptr<Player> player;
			std::shared_ptr<UIElement> background;
			std::shared_ptr<UIElement> currentMenu;

			float t;
			float dt;
			float currentTime;
			float accumulator;

			std::vector<std::shared_ptr<TestEnemy>> enemies;
			std::vector<std::shared_ptr<Explosion>> explosions;
			std::unordered_map<std::string, std::shared_ptr<UIElement>> ui;
			std::unordered_map<std::string, std::shared_ptr<UIElement>> playerUI;

			std::shared_ptr<CollisionManager> collisionManager;
			std::shared_ptr<Renderer> renderer;
			std::shared_ptr<TextureManager> textureManager;
			std::shared_ptr<InputManager> inputManager;
			std::shared_ptr<FontManager> fontManager;
			GameState gameState;
	};
}
#endif