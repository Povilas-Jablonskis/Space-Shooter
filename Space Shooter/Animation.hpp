#ifndef animationH
#define animationH

#include <glew/glew.h>
#include <glm/vec4.hpp>
#include <vector>

namespace Engine
{
	class Animation
	{
		public:
			Animation(GLuint, int, int);
			void addSprite(const glm::vec4& sprite) { m_sprites.push_back(sprite); }
			const std::vector<glm::vec4>& getAnimation() const { return m_sprites; }
			GLuint getSpriteSheetTexture() const { return m_spriteSheetTexture; }
			int getSpriteSheetWidth() const { return m_spriteSheetWidth; };
			int getSpriteSheetHeight() const { return m_spriteSheetHeight; };
			void loadFromFile(const std::string&);
		private:
			void setSpriteSheetWidth(int t_spriteSheetWidth) { m_spriteSheetWidth = t_spriteSheetWidth; };
			void setSpriteSheetHeight(int t_spriteSheetHeight) { m_spriteSheetHeight = t_spriteSheetHeight; };

			int m_spriteSheetWidth;
			int m_spriteSheetHeight;
			GLuint m_spriteSheetTexture;
			std::vector<glm::vec4> m_sprites;
	};
}
#endif