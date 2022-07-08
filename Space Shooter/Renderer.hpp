#ifndef rendererH
#define rendererH

#include "Shader.hpp"
#include "Text.hpp"

#include <string>
#include <memory>
#include <unordered_map>
#include <glew/glew.h>

class Sprite;

class Renderer
{
public:
	Renderer();
	void addShader(const std::string&, const std::shared_ptr<Shader>&);

	void draw(const std::vector<std::shared_ptr<Text>>&) const;
	void draw(const Text&) const;

	void draw(const Sprite&) const;
private:
	GLuint m_vbo{}, m_vao{}, m_ebo{};
	GLuint m_textVBO{};
	GLuint m_textVAO{};
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};
#endif