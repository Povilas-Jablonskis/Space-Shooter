#ifndef spriteSheetH
#define spriteSheetH

#define STB_IMAGE_IMPLEMENTATION

#include <glm/vec4.hpp>
#include <glew/glew.h>
#include <unordered_map>
#include <memory>
#include <string>

class Animation;

class SpriteSheet
{
public:
	SpriteSheet(const std::string&, const std::string&);
	std::shared_ptr<Animation> getSprite(const std::string&);
	glm::vec4 getSpriteAsVector(const std::string&) const;
	std::shared_ptr<Animation> getAnimation(const std::string&) const;
	int getWidth() const { return m_width; };
	int getHeight() const { return m_height; };
	void setWidth(const int width) { m_width = width; }
	void setHeight(const int height) { m_height = height; }
	void makeAnimation(const std::string&, const std::vector<std::string>&);
	void makeAnimation(const std::string&, const std::vector<glm::vec4>&);
	GLuint getTexture() const { return m_texture; }
private:
	void loadSpriteSheet(const std::string&);
	void loadSpritesFromXml(const std::string&);

	int m_width{};
	int m_height{};
	GLuint m_texture{};
	std::unordered_map<std::string, glm::vec4> m_sprites;
	std::unordered_map<std::string, std::shared_ptr<Animation>> m_animations;
};
#endif