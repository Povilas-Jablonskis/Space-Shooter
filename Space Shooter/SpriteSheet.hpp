#ifndef spriteSheetH
#define spriteSheetH

#define STB_IMAGE_IMPLEMENTATION

#include <glm/vec4.hpp>
#include <glew/glew.h>
#include <unordered_map>
#include <string>

class SpriteSheet
{
public:
	explicit SpriteSheet(const std::string&);
	[[nodiscard]] const glm::vec4& getSprite(const std::string&) const;
	[[nodiscard]] int getWidth() const;
	[[nodiscard]] int getHeight() const;
	[[nodiscard]] GLuint getTexture() const;

private:
	bool loadSpritesFromXml(const std::string&);
	void loadSpriteSheet(const std::string&);

	int m_width{};
	int m_height{};
	GLuint m_texture{};
	std::unordered_map<std::string, glm::vec4> m_sprites;
};
#endif
