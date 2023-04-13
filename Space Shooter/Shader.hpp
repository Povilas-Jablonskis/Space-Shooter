#ifndef shaderH
#define shaderH

#include <string>
#include <glew/glew.h>

class Shader
{
public:
	Shader(const std::string&, const std::string&);
	void load(const std::string&, const std::string&);
	[[nodiscard]] GLuint get() const;

private:
	[[nodiscard]] std::string loadFromFile(const std::string&);
	GLuint m_program{};
};
#endif
