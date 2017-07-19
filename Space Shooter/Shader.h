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
			Shader(const std::string&, const std::string&);
			~Shader();
			void loadShader(const std::string&, const std::string&);
			std::string readShaderFile(const std::string&);
			inline GLuint getShader() const { return program; }
		private:
			GLuint program;
	};
}
#endif