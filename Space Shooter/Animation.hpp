#ifndef animationH
#define animationH

#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <glm/vec4.hpp>
#include <vector>
#include <Simple OpenGL Image Library/SOIL.h>

namespace Engine
{
	class Animation
	{
		public:
			Animation(GLuint, int, int);
			inline void addSprite(const glm::vec4& sprite) { m_sprites.push_back(sprite); }
			inline const std::vector<glm::vec4>& getAnimation() const { return m_sprites; }
			inline GLuint getSpriteSheetTexture() const { return m_spriteSheetTexture; }
			inline int getSpriteSheetWidth() const { return m_spriteSheetWidth; };
			inline int getSpriteSheetHeight() const { return m_spriteSheetHeight; };
			void loadFromFile(const std::string&);
		private:
			inline void setSpriteSheetWidth(int t_spriteSheetWidth) { m_spriteSheetWidth = t_spriteSheetWidth; };
			inline void setSpriteSheetHeight(int t_spriteSheetHeight) { m_spriteSheetHeight = t_spriteSheetHeight; };

			int m_spriteSheetWidth;
			int m_spriteSheetHeight;
			GLuint m_spriteSheetTexture;
			std::vector<glm::vec4> m_sprites;
	};
}
#endif