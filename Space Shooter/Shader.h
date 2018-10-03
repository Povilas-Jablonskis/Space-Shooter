#ifndef _shaderH
#define _shaderH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"

namespace Engine
{
	class Shader
	{
		public:
			~Shader();
			Shader(std::string, std::string);
			void loadShader(std::string, std::string);
			std::string readShaderFile(std::string);
			inline GLuint getShader()  { return program; }
		private:
			GLuint program;
	};
}
#endif