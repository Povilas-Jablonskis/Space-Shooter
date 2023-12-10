#ifndef shaderH
#define shaderH

#include <string>
#include <glew/glew.h>

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	void load(const std::string& vertexPath, const std::string& fragmentPath);
	[[nodiscard]] GLuint get() const;

private:
	[[nodiscard]] std::string loadFromFile(const std::string& path);
	GLuint m_program{};
};
#endif
