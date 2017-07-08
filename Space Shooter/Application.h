#ifndef _applicationH
#define _applicationH

#include <map>

#include "FontManager.h"
#include "Shader.h"
#include "GameState.h"
#include "InputManager.h"

namespace Engine
{
	class Application
	{
		public:
			~Application();
			Application();
			void AddShader(const std::string&, Shader*);
			FT_Face* GetFont(const std::string&);
			FT_Face* LoadFont(const std::string&, const std::string&);
			GameState GetState()  const;
			void SetState(GameState);
			InputManager& GetInputManager();
			static const GLuint GetVAO();
			static const GLuint GetShaderProgram(const std::string&);
			static const GLuint GetTextVBO();
			static const GLuint GetTextTexture();
			static GLuint indices[6];
			static GLfloat vertices[16];
		private:
			InputManager* inputManager;
			GameState gamestate;
			FontManager* fontManager;
			static GLuint VBO, VAO, EBO;
			static GLuint TextVBO;
			static GLuint TextTexture;
			static std::map<std::string, Shader*> shaders;
		};
}
#endif