#include "UIElementBase.h"
#include "Application.h"

namespace Engine
{
	UIElementBase::UIElementBase(int _width, int _height, glm::vec2 _position, glm::vec4 _color) :
		width(_width), height(_height), position(_position), color(_color), texture(nullptr)
	{
		initFuncs();
	}

	UIElementBase::~UIElementBase()
	{
		if (texture != nullptr)
		{
			auto _texture = texture->getTexture();
			glDeleteTextures(0, &_texture);
		}
	}

	void UIElementBase::initFuncs()
	{
		onHoverEnterFunc = []()
		{

		};

		onHoverExitFunc = []()
		{

		};

		onMouseClickFunc = []()
		{

		};

		onMouseReleaseFunc = []()
		{

		};
	}

	void UIElementBase::draw()
	{
		auto program = Application::getShaderProgram("shader");

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(Application::getVAO());
			glUseProgram(program);
				float windowwidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
				float windowheigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

				int offsetLocation = glGetUniformLocation(program, "givenposition");
				int offsetLocation2 = glGetUniformLocation(program, "size");
				int offsetLocation3 = glGetUniformLocation(program, "color");
				int offsetLocation4 = glGetUniformLocation(program, "renderMode");
				int offsetLocation5 = glGetUniformLocation(program, "animscX");
				int offsetLocation6 = glGetUniformLocation(program, "animscY");
				int offsetLocation7 = glGetUniformLocation(program, "curranim");

				if (texture != nullptr)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, texture->getTexture());

					glUniform1f(offsetLocation4, 1.0f);
					glUniform1f(offsetLocation5, texture->getCount().x);
					glUniform1f(offsetLocation6, texture->getCount().y);
					glUniform1f(offsetLocation7, (float)texture->getCurrentFrame());
				}
				else
					glUniform1f(offsetLocation4, 0.0f);

				glUniform2f(offsetLocation, position.x / windowwidth, position.y / windowheigth);
				glUniform2f(offsetLocation2, width / windowwidth, height / windowheigth);
				glUniform4f(offsetLocation3, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a);
				glDrawElements(GL_TRIANGLES, (sizeof(Application::indices) / sizeof(*Application::indices)), GL_UNSIGNED_INT, 0);
			glUseProgram(0);
		glBindVertexArray(0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	float UIElementBase::getPosition(int index) const
	{
		return position[index];
	}

	int UIElementBase::getSize(int index) const
	{
		switch (index)
		{
			case 0:
			{
				return width;
			}
			case 1:
			{
				return height;
			}
			default:
			{
				return NULL;
			}
		}
	}

	float UIElementBase::getColor(int index) const
	{
		return color[index];
	}

	void UIElementBase::changeColor(float _color, int index)
	{
		color[index] = _color;
	}

	void UIElementBase::onMouseClickDefaults(InputManager* inputManager)
	{
		if (color.a == 0.0f) return;
		glm::vec2 lastMousePosition = inputManager->getLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= position.x && lastMousePosition.x <= (position.x + width) && lastMousePosition.y <= position.y && lastMousePosition.y >= (position.y + height))
			onMouseClickFunc();
	}

	void UIElementBase::onMouseReleaseFuncDefaults(InputManager* inputManager)
	{
		if (color.a == 0.0f) return;
		glm::vec2 lastMousePosition = inputManager->getLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= position.x && lastMousePosition.x <= (position.x + width) && lastMousePosition.y <= position.y && lastMousePosition.y >= (position.y + height))
			onMouseReleaseFunc();
	}

	void UIElementBase::applyTexture(Texture* _texture)
	{
		auto tempTexture = new Texture();
		*tempTexture = *_texture;
		texture = tempTexture;
	}

	void UIElementBase::setCurrentTextureCurrentFrame(int frame)
	{
		if (texture != nullptr)
			texture->setCurrentFrame(frame);
	}

	void UIElementBase::update(InputManager* inputManager, float dt)
	{
		if (texture != nullptr)
			texture->update(dt);
	}
}