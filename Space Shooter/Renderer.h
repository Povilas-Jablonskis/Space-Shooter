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
			Renderer();
			~Renderer();
			void Render(Player*);
			void Render(BaseGameObject*);
			void Render(Bullet*);
			void Render(UIElement*);
			void Render(Text*, UIElement*);
			void AddShader(const std::string&, Shader*);
		private:
			GLuint* indices;
			GLfloat* vertices;
			GLuint VBO, VAO, EBO;
			std::map<std::string, Shader*> shaders;
	};
}
#endif