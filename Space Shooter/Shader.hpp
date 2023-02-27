#ifndef shaderH
#define shaderH

#include <string>
#include <glew/glew.h>

class Shader
{
public:
	Shader(const std::string&, const std::string&);
	void loadShader(const std::string&, const std::string&);
	[[nodiscard]] static std::string readShaderFile(const std::string&);
	[[nodiscard]] GLuint getShader() const;

private:
	GLuint m_program{};
};
#endif
