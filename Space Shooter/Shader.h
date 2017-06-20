#ifndef _Shader
#define _Shader

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"

namespace Engine
{
	class Shader
	{
		public:
			Shader(std::string, std::string);
			Shader();
			~Shader();
			void LoadShader(std::string, std::string);
			std::string ReadShaderFile(std::string);
			GLuint GetShader();
		private:
			GLuint program;
	};
}
#endif