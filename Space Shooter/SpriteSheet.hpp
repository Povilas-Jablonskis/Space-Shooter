#ifndef spriteSheetH
#define spriteSheetH

#include <glm/vec4.hpp>
#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <fstream>
#include "rapidxml/rapidxml.hpp"
#include <algorithm>
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
			~SpriteSheet();
			SpriteSheet() = default;
			SpriteSheet(const SpriteSheet &) = default;
			SpriteSheet& operator=(const SpriteSheet&) = default;
			SpriteSheet(SpriteSheet&&) = default;
			SpriteSheet& operator=(SpriteSheet&&) = default;
			void loadSpriteSheet(const std::string&);
			void loadSpritesFromXml(const std::string&);
			std::shared_ptr<Animation> getSprite(const std::string&);
			glm::vec4 getSpriteAsVector(const std::string&);
			std::shared_ptr<Animation> getAnimation(const std::string&);
			inline int getWidth() const { return m_width; };
			inline int getHeight() const { return m_height; };
			inline void setWidth(int width) { m_width = width; }
			inline void setHeight(int height) { m_height = height; }
			void makeAnimation(const std::string&, const std::vector<std::string>&);
			void makeAnimation(const std::string&, const std::vector<glm::vec4>&);
			inline GLuint getTexture() const { return m_texture; }
		private:
			inline const std::vector<sprite>& getSprites() const { return m_sprites; }
			inline const std::vector<animation>& getAnimations() const { return m_animations; }

			int m_width;
			int m_height;
			GLuint m_texture;
			std::vector<sprite> m_sprites;
			std::vector<animation> m_animations;
	};
}
#endif