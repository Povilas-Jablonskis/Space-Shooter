#ifndef spriteSheetH
#define spriteSheetH

#include <glm/vec4.hpp>
#include <glew/glew.h>
#include <vector>
#include <memory>

namespace Engine
{
	class Animation;

	typedef std::pair<std::string, glm::vec4> sprite;
	typedef std::pair<std::string, std::shared_ptr<Animation>> animation;

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
			const std::vector<sprite>& getSprites() const { return m_sprites; }
			const std::vector<animation>& getAnimations() const { return m_animations; }

			int m_width{};
			int m_height{};
			GLuint m_texture{};
			std::vector<sprite> m_sprites;
			std::vector<animation> m_animations;
	};
}
#endif