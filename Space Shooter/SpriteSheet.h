#ifndef _spriteSheetH
#define _spriteSheetH

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "SOIL.h"
#include <string>
#include <vec4.hpp>
#include <vector>
#include <rapidxml.hpp>
#include <iostream>
#include <memory>

#include "Animation.h"

namespace Engine
{
	typedef std::pair<std::string, glm::vec4> sprite;
	typedef std::pair<std::string, std::shared_ptr<Animation>> animation;

	class SpriteSheet
	{
		public:
			~SpriteSheet();
			void loadSpriteSheet(const std::string&);
			void loadSpritesFromXml(const std::string&);
			std::shared_ptr<Animation> getSprite(const std::string&);
			glm::vec4 getSpriteAsVector(const std::string&);
			std::shared_ptr<Animation> getAnimation(const std::string&);
			int getSize(int) const;
			void makeAnimation(const std::string&, std::vector<std::string>);
			void makeAnimation(const std::string&, std::vector<glm::vec4>);
			GLuint getTexture() const { return texture; }
		private:
			int width;
			int height;
			GLuint texture;
			std::vector<sprite> sprites;
			std::vector<animation> animations;
	};
}
#endif