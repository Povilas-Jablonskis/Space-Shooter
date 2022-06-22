#ifndef texture_hpp
#define texture_hpp

#include <string>
#include <glm/vec2.hpp>
#include <glew/glew.h>

class Texture
{
public:
	bool loadFromFile(const std::string&);
	const glm::vec2& getSize() const { return m_size; };
	const GLuint getTexture() const { return m_texture; };
private:
	glm::vec2 m_size{};
	GLuint m_texture{};
};

#endif