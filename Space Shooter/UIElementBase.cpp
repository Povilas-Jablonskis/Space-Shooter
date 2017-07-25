#include "UIElementBase.h"
#include "Application.h"

namespace Engine
{
	UIElementBase::UIElementBase(int _width, int _height, glm::vec2 _position, glm::vec4 _color, glm::vec2 _positionPerc) :
		width(_width), height(_height), position(_position), color(_color), texture(nullptr), positionPercents(_positionPerc), animComplete(false), animTimer(0.0f), loop(false), delay(1.0f), currentFrame(0)
	{
		initFuncs();
	}

	UIElementBase::~UIElementBase()
	{

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

	void UIElementBase::draw(GLuint program)
	{
		if (color.a == 0.0f) return;

		float windowwidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowheigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		int offsetLocation = glGetUniformLocation(program, "color");
		int offsetLocation2 = glGetUniformLocation(program, "renderMode");
		int offsetLocation3 = glGetUniformLocation(program, "animscX");
		int offsetLocation4 = glGetUniformLocation(program, "animscY");
		int offsetLocation5 = glGetUniformLocation(program, "curranim");
		int offsetLocation6 = glGetUniformLocation(program, "projection");
		int offsetLocation7 = glGetUniformLocation(program, "model");

		glm::mat4 projection = glm::ortho(0.0f, (float)glutGet(GLUT_WINDOW_WIDTH), 0.0f, (float)glutGet(GLUT_WINDOW_HEIGHT), 0.0f, 1.0f);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(position, 0.0f));

		/*model = glm::translate(model, glm::vec3(0.5f * width, 0.5f * height, 0.0f));
		model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * width, -0.5f * height, 0.0f));*/

		model = glm::scale(model, glm::vec3(width, height, 1.0f));

		glUniform4f(offsetLocation, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a);

		if (texture != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D, texture->getTexture());

			glUniform1f(offsetLocation2, 1.0f);
			glUniform1f(offsetLocation3, texture->getCount().x);
			glUniform1f(offsetLocation4, texture->getCount().y);
			glUniform1f(offsetLocation5, (float)getCurrentFrame());
		}
		else
			glUniform1f(offsetLocation2, 0.0f);

		glUniformMatrix4fv(offsetLocation6, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(offsetLocation7, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

	void UIElementBase::onMouseClickDefaults(std::shared_ptr<InputManager> inputManager)
	{
		if (color.a == 0.0f) return;
		glm::vec2 lastMousePosition = inputManager->getLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= position.x && lastMousePosition.x <= (position.x + width) && lastMousePosition.y <= position.y && lastMousePosition.y >= (position.y + height))
			onMouseClickFunc();
	}

	void UIElementBase::onMouseReleaseFuncDefaults(std::shared_ptr<InputManager> inputManager)
	{
		if (color.a == 0.0f) return;
		glm::vec2 lastMousePosition = inputManager->getLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= position.x && lastMousePosition.x <= (position.x + width) && lastMousePosition.y <= position.y && lastMousePosition.y >= (position.y + height))
			onMouseReleaseFunc();
	}

	void UIElementBase::applyTexture(std::shared_ptr<Texture> _texture)
	{
		if (_texture == nullptr || _texture == texture) return;
		texture = _texture;
	}

	void UIElementBase::updateTexture(float dt)
	{
		if (texture == nullptr) return;
		if (texture->getEndFrame() - texture->getStartFrame() > 0)
		{
			animTimer += dt;
			if (animTimer > delay)
			{
				animTimer -= delay;
				currentFrame++;
				if (currentFrame < texture->getStartFrame() || currentFrame > texture->getEndFrame())
				{
					if (loop == true)
						currentFrame = texture->getStartFrame();
					else
					{
						currentFrame = texture->getEndFrame();
						animComplete = true;
					}
				}
			}
		}
	}

	void UIElementBase::update(std::shared_ptr<InputManager> inputManager, float dt)
	{
		if (color.a == 0.0f) return;
		if (texture != nullptr)
			updateTexture(dt);
	}

	void UIElementBase::fixPosition(UIElementBase* parent)
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));
		if (parent != nullptr)
		{
			if (positionPercents == glm::vec2(0.0f, 0.0f))
			{
				position.x = parent->getPosition(0);
				position.y = parent->getPosition(1);
			}
			else
			{
				position.x = parent->getPosition(0) + (parent->getSize(0) * (positionPercents.x / 100.0f));
				position.y = parent->getPosition(1) + (parent->getSize(1) * (positionPercents.y / 100.0f));
			}
		}
		else
		{
			if (positionPercents == glm::vec2(0.0f, 0.0f))
			{
				width = (int)temPos.x;
				height = (int)temPos.y;
				return;
			}
			position.x = temPos.x * (positionPercents.x / 100.0f);
			position.y = temPos.y * (positionPercents.y / 100.0f);
		}
	}
}