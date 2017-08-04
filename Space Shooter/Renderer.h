#ifndef _rendererH
#define _rendererH

#include <unordered_map>
#include <memory>
#include <algorithm>

#include "Shader.h"
#include "Text.h"
#include "RenderObject.h"
#include "Font.h"

namespace Engine
{
	class Renderer
	{
		public:
			~Renderer();
			Renderer();
			void addShader(const std::string& name, std::shared_ptr<Shader> shader);
			inline GLuint getVAO() const { return vao; }
			GLuint getShaderProgram(const std::string&) const;
			inline GLuint getTextVBO() const { return textVBO; }
			inline GLuint getTextVAO() const { return textVAO; }
			void draw(std::vector<std::shared_ptr<RenderObject>>);
			void draw(std::shared_ptr<RenderObject>);
			void draw(std::vector<std::shared_ptr<Text>>);
			void draw(std::shared_ptr<Text>);
		private:
			GLuint vbo, vao, ebo;
			GLuint textVBO;
			GLuint textVAO;
			std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
	};
}
#endif