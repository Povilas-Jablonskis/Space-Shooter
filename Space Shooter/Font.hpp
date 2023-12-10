#ifndef fontH
#define fontH

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glew/glew.h>
#include <glm/vec2.hpp>
#include <unordered_map>
#include <string>

struct Character
{
	GLuint textureID; // ID handle of the glyph texture
	glm::vec2 size; // Size of glyph
	glm::vec2 bearing; // Offset from baseline to left/top of glyph
	FT_Pos advance; // Offset to advance to next glyph
};

class Font
{
public:
	explicit Font(const std::string& path);
	[[nodiscard]] Character getCharacter(const FT_ULong index) const;

private:
	std::unordered_map<FT_ULong, Character> m_characters;
};
#endif
