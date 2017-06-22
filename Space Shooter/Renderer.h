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

namespace Engine
{
	class Renderer
	{
	public:
		static void Render(Player);
		static void Render(TestEnemy*);
		static void Render(BaseGameObject*);
		static void Render(Text*);
		static void Render(UIElementBase* UIElementBase);
		static void Init();
		static void AddShader(const char*, Shader*);
	private:
		static GLuint indices[6];
		static GLfloat vertices[16];
		static GLuint VBO, VAO, EBO;
		static std::map<const char*, Shader*> shaders;
	};
}
#endif