#ifndef rendererH
#define rendererH

#include "Shader.hpp"
#include "Text.hpp"
#include "Sprite.hpp"

#include <string>
#include <memory>
#include <unordered_map>
#include <glew/glew.h>

class Renderer
{
public:
	Renderer();
	void addShader(const std::string& name, const std::shared_ptr<Shader>& shader);

	void draw(const std::vector<std::shared_ptr<Text>>& texts) const;
	void draw(const Text& text) const;

	void draw(const Sprite& sprite) const;

private:
	GLuint m_vbo{}, m_vao{}, m_ebo{};
	GLuint m_textVBO{};
	GLuint m_textVAO{};
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};
#endif
