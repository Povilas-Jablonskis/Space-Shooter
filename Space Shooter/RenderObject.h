#ifndef _renderObjectH
#define _renderObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <functional>
#include <memory>
#include <map>

#include "Texture.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class RenderObject
	{
		public:
			~RenderObject();
			RenderObject(int, int, glm::vec2, glm::vec4);
			inline void changeColor(float _color, int index) { color[index] = _color; }
			inline float getColor(int index) const{ return color[index]; }
			inline float getPosition(int index) const { return position[index]; }
			inline glm::vec2 getPosition() const { return position; }
			inline std::shared_ptr<Texture> getTexture() { return texture; }
			inline void setPosition(glm::vec2 _position) { position = _position; }
			inline void setPosition(int index, float _position) { position[index] = _position; }
			int getSize(int) const;
			virtual void updateTexture(float);
			void applyTexture(std::shared_ptr<Texture>);
		protected:
			std::shared_ptr<Texture> texture;
			glm::vec2 position;
			int width;
			int height;
			glm::vec4 color;
		};
}
#endif