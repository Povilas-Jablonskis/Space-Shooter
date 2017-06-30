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
			Shader(const std::string&, const std::string&g);
			Shader();
			~Shader();
			void LoadShader(const std::string&, const std::string&);
			std::string ReadShaderFile(const std::string&);
			const GLuint GetShader();
		private:
			GLuint program;
	};
}
#endif