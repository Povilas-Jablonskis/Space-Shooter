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
			Shader();
			~Shader();
			void LoadShader(const std::string&, const std::string&);
			std::string ReadShaderFile(const std::string&);
			GLuint GetShader() const;
		private:
			GLuint program;
	};
}
#endif