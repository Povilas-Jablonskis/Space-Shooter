#ifndef _applicationH
#define _applicationH

#include <map>

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
			void addShader(const std::string&, Shader*);
			void loadTexture(const std::string&, const std::string&, glm::vec2);
			Texture getTexture(const std::string&);
			FT_Face* getFont(const std::string&);
			FT_Face* loadFont(const std::string&, const std::string&);
			GameState getState()  const;
			void setState(GameState);
			InputManager& getInputManager();
			static const GLuint getVAO();
			static const GLuint getShaderProgram(const std::string&);
			static const GLuint getTextVBO();
			static const GLuint getTextTexture();
			static GLuint indices[6];
			static GLfloat vertices[16];
		private:
			TextureManager* textureManager;
			InputManager* inputManager;
			GameState gameState;
			FontManager* fontManager;
			static GLuint vbo, vao, ebo;
			static GLuint textVBO;
			static GLuint textTexture;
			static std::map<std::string, Shader*> shaders;
		};
}
#endif