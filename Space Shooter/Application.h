#ifndef _applicationH
#define _applicationH

#include <map>
#include <memory>

#include "FontManager.h"
#include "Shader.h"
#include "GameState.h"
#include "InputManager.h"
#include "TextureManager.h"

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
			FT_FaceRec_* getFont(const std::string&);
			FT_FaceRec_* loadFont(const std::string&, const std::string&);
			GameState getState() const;
			void setState(GameState);
			InputManager* getInputManager();
			static const GLuint getVAO();
			static const GLuint getShaderProgram(const std::string&);
			static const GLuint getTextVBO();
			static const GLuint getTextTexture();
			static GLuint indices[6];
			static GLfloat vertices[16];
		private:
			std::shared_ptr<TextureManager> textureManager;
			std::shared_ptr<InputManager> inputManager;
			GameState gameState;
			std::shared_ptr<FontManager> fontManager;
			static GLuint vbo, vao, ebo;
			static GLuint textVBO;
			static GLuint textTexture;
			static std::map<std::string, std::shared_ptr<Shader>> shaders;
		};
}
#endif