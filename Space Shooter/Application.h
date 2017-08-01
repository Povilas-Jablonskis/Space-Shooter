#ifndef _applicationH
#define _applicationH

#include <unordered_map>
#include <memory>

#include "Renderer.h"
#include "FontManager.h"
#include "GameState.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "CollisionManager.h"

namespace Engine
{
	class Application
	{
		public:
			~Application();
			Application();
			void addShader(const std::string&, std::shared_ptr<Shader>);
			void loadTexture(const std::string&, const std::string&, int, int, glm::vec2);
			std::shared_ptr<Texture> getTexture(const std::string&);
			inline std::shared_ptr<Font> getFont(const std::string& name) { return fontManager->getFont(name); }
			void loadFont(const std::string&, const std::string&);
			inline GameState getState() const { return gameState; }
			inline void setState(GameState state) { gameState = state; }
			inline std::shared_ptr<InputManager> getInputManager() const { return inputManager; }
			inline std::shared_ptr<TextureManager> getTextureManager() const { return textureManager; }
			inline std::shared_ptr<Renderer> getRender() const { return renderer; }
			inline std::shared_ptr<CollisionManager> getCollisionManager() const { return collisionManager; }
		private:
			std::shared_ptr<CollisionManager> collisionManager;
			std::shared_ptr<Renderer> renderer;
			std::shared_ptr<TextureManager> textureManager;
			std::shared_ptr<InputManager> inputManager;
			std::shared_ptr<FontManager> fontManager;
			GameState gameState;
	};
}
#endif