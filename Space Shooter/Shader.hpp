#ifndef shaderH
#define shaderH

#include <iostream>
#include <fstream>
#include <vector>
#include <glew/glew.h>
#include <freeglut/freeglut.h>

namespace Engine
{
	class Shader
	{
		public:
			Shader(const std::string&, const std::string&);
			void loadShader(const std::string&, const std::string&);
			std::string readShaderFile(const std::string&) const;
			inline GLuint getShader() const { return m_program; }
		private:
			inline void setShader(GLuint t_program) { m_program = t_program; }
			GLuint m_program;
	};
}
#endif