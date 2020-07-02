#ifndef shaderH
#define shaderH

#include <vector>
#include <string>
#include <glew/glew.h>

namespace Engine
{
	class Shader
	{
		public:
			Shader(const std::string&, const std::string&);
			void loadShader(const std::string&, const std::string&);
			std::string readShaderFile(const std::string&) const;
			GLuint getShader() const { return m_program; }
		private:
			void setShader(const GLuint t_program) { m_program = t_program; }
			GLuint m_program{};
	};
}
#endif