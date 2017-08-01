#ifndef _rendererH
#define _rendererH

#include <unordered_map>
#include <memory>

#include "Shader.h"
#include "Player.h"
#include "Text.h"
#include "UIElementBase.h"
#include "BaseGameObject.h"

namespace Engine
{
	class Renderer
	{
		public:
			Renderer();
			~Renderer();
			void addShader(const std::string& name, std::shared_ptr<Shader> shader);
			inline GLuint getVAO() const { return vao; }
			GLuint getShaderProgram(const std::string&) const;
			inline GLuint getTextVBO() const { return textVBO; }
			inline GLuint getTextVAO() const { return textVAO; }
			void draw(std::vector<std::shared_ptr<UIElementBase>>);
			void draw(std::shared_ptr<UIElementBase>);
			void draw(std::vector<std::shared_ptr<BaseGameObject>>);
			void draw(std::shared_ptr<BaseGameObject>);
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