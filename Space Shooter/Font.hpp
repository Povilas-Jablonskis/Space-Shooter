#ifndef fontH
#define fontH

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glew/glew.h>
#include <glm/vec2.hpp>
#include <vector>

struct Character
{
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::vec2  Size;       // Size of glyph
	glm::vec2  Bearing;    // Offset from baseline to left/top of glyph
	FT_Pos     Advance;    // Offset to advance to next glyph
};

typedef std::pair<GLchar, Character> character;

class Font
{
public:
	explicit Font(const FT_Face&);
	Character getCharacter(GLchar) const;
	const std::vector<character>& getCharacterList() const { return m_characters; }
private:
	const FT_Face& getFace() const { return m_face; }

	FT_Face m_face;
	void loadCharacters();
	std::vector<character> m_characters;
};
#endif