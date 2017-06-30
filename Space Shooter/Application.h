#ifndef _Application
#define _Application

#include <map>

#include "BulletManager.h"
#include "Shader.h"

namespace Engine
{
	class Application
	{
		public:
			~Application();
			Application();
			std::vector<std::shared_ptr<Bullet>>* GetBulletsList();
			void AddShader(const std::string&, Shader*);
			static const GLuint GetVAO();
			static const GLuint GetShaderProgram(const std::string&);
			static const GLuint GetTextVBO();
			static const GLuint GetTextTexture();
			static GLuint indices[6];
			static GLfloat vertices[16];
		private:
			BulletManager* bulletManager;
			static GLuint VBO, VAO, EBO;
			static GLuint TextVBO;
			static GLuint TextTexture;
			static std::map<std::string, Shader*> shaders;
		};
}
#endif