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
			void loadSpriteSheet(std::string);
			void loadSpritesFromXml(std::string);
			std::shared_ptr<Animation> getSprite(std::string);
			glm::vec4 getSpriteAsVector(std::string);
			std::shared_ptr<Animation> getAnimation(std::string);
			inline int getWidth()  { return width; };
			inline int getHeight()  { return height; };
			inline void setWidth(int _width) { width = _width; }
			inline void setHeight(int _height) { height = _height; }
			void makeAnimation(std::string, std::vector<std::string>);
			void makeAnimation(std::string, std::vector<glm::vec4>);
			GLuint getTexture()  { return texture; }
		private:
			int width;
			int height;
			GLuint texture;
			std::vector<sprite> sprites;
			std::vector<animation> animations;
	};
}
#endif