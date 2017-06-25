#ifndef _Renderer
#define _Renderer

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <map>

#include "Player.h"
#include "TestEnemy.h"
#include "Text.h"
#include "UIElement.h"
#include "BaseGameObject.h"
#include "Shader.h"
#include "Bullet.h"

namespace Engine
{
	class Renderer
	{
		public:
			static void Render(Player);
			static void Render(BaseGameObject*);
			static void Render(Bullet*);
			static void Render(UIElement*);
			static void Render(Text*, UIElement*);
			static void Init();
			static void AddShader(std::string, Shader*);
		private:
			static GLuint indices[6];
			static GLfloat vertices[16];
			static GLuint VBO, VAO, EBO;
			static std::map<std::string, Shader*> shaders;
	};
}
#endif