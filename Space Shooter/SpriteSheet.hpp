#ifndef spriteSheetH
#define spriteSheetH

#define STB_IMAGE_IMPLEMENTATION

#include <glm/vec4.hpp>
#include <glew/glew.h>
#include <unordered_map>
#include <memory>
#include <string>

class SpriteSheet
{
public:
	SpriteSheet(const std::string&);
	const glm::vec4& getSprite(const std::string&) const;
	int getWidth() const;
	int getHeight() const;
	GLuint getTexture() const;
private:
	bool loadSpritesFromXml(const std::string&);
	void loadSpriteSheet(const std::string&);

	int m_width{};
	int m_height{};
	GLuint m_texture{};
	std::unordered_map<std::string, glm::vec4> m_sprites;
};
#endif