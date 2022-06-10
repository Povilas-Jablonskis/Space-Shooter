#ifndef shaderH
#define shaderH

#include <vector>
#include <string>
#include <glew/glew.h>

class Shader
{
public:
	Shader(const std::string&, const std::string&);
	void loadShader(const std::string&, const std::string&);
	std::string readShaderFile(const std::string&) const;
	GLuint getShader() const { return m_program; }
private:
	GLuint m_program{};
};
#endif