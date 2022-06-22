#ifndef fontH
#define fontH

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glew/glew.h>
#include <glm/vec2.hpp>
#include <unordered_map>

struct Character
{
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::vec2  Size;       // Size of glyph
	glm::vec2  Bearing;    // Offset from baseline to left/top of glyph
	FT_Pos     Advance;    // Offset to advance to next glyph
};

class Font
{
public:
	explicit Font(const FT_Face&);
	Character getCharacter(FT_ULong) const;
private:
	FT_Face m_face{};
	std::unordered_map<FT_ULong, Character> m_characters;
};
#endif