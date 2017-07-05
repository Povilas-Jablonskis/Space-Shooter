#ifndef _applicationH
#define _applicationH

#include <map>

#include "FontManager.h"
#include "BulletManager.h"
#include "Shader.h"
#include "GameState.h"

namespace Engine
{
	class Application
	{
		public:
			~Application();
			Application();
			std::vector<std::shared_ptr<Bullet>>* GetBulletsList();
			void AddShader(const std::string&, Shader*);
			FT_Face* GetFont(const std::string&);
			FT_Face* LoadFont(const std::string&, const std::string&);
			GameState GetState()  const;
			bool GetKey(char) const;
			void SetState(GameState);
			void SetKey(char, bool);
			void SetKey(int, bool);
			void UpdateDeltaTime();
			static const GLuint GetVAO();
			static const GLuint GetShaderProgram(const std::string&);
			static const GLuint GetTextVBO();
			static const GLuint GetTextTexture();
			static GLuint indices[6];
			static GLfloat vertices[16];
		private:
			bool pressedkeys[256];
			int prevtime, currtime;
			float dt;
			GameState gamestate;
			BulletManager* bulletManager;
			FontManager* fontManager;
			static GLuint VBO, VAO, EBO;
			static GLuint TextVBO;
			static GLuint TextTexture;
			static std::map<std::string, Shader*> shaders;
		};
}
#endif