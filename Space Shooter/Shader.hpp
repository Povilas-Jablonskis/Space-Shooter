#ifndef shaderH
#define shaderH

#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

namespace Engine
{
	class Shader
	{
		public:
			~Shader();
			Shader(const std::string&, const std::string&);
			Shader(const Shader &) = default;
			Shader& operator=(const Shader&) = default;
			Shader(Shader&&) = default;
			Shader& operator=(Shader&&) = default;
			void loadShader(const std::string&, const std::string&);
			std::string readShaderFile(const std::string&) const;
			inline GLuint getShader() const { return m_program; }
		private:
			inline void setShader(GLuint t_program) { m_program = t_program; }
			GLuint m_program;
	};
}
#endif