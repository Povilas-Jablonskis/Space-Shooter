#include "UIElementBase.h"

namespace Engine
{
	UIElementBase::UIElementBase(int _width, int _height, glm::vec2 _position, glm::vec4 _color, glm::vec2 _positionPerc) :
		width(_width), height(_height), position(_position), color(_color), gotMousedHovered(false), texture(nullptr), isStatic(false), positionPercents(_positionPerc)
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
			glUniform1f(offsetLocation5, (float)texture->getCurrentFrame());
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

	void UIElementBase::applyTexture(std::shared_ptr<Texture> _texture)
	{
		if (_texture == nullptr) return;
		_texture->setAnimTimer(0.0f);
		texture = _texture;
	}

	void UIElementBase::updateTexture(float dt)
	{
		if (texture == nullptr) return;
		if (texture->getEndFrame() - texture->getStartFrame() > 0)
		{
			texture->setAnimTimer(texture->getAnimTimer() + dt);
			if (texture->getAnimTimer() > texture->getDelay())
			{
				texture->setAnimTimer(texture->getAnimTimer() - texture->getDelay());
				texture->setCurrentFrame(texture->getCurrentFrame() + 1);
				if (texture->getCurrentFrame() < texture->getStartFrame() || texture->getCurrentFrame() > texture->getEndFrame())
				{
					if (texture->getLoopStatus())
						texture->setCurrentFrame(texture->getStartFrame());
					else
					{
						texture->setCurrentFrame(texture->getEndFrame());
						texture->setAnimationStatus(true);
					}
				}
			}
		}
	}

	void UIElementBase::update(float dt)
	{
		if (color.a == 0.0f) return;
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

	bool UIElementBase::checkIfCollides(glm::vec2 colCoordinates)
	{
		if (colCoordinates.x >= position.x && colCoordinates.x <= (position.x + width) && colCoordinates.y <= position.y && colCoordinates.y >= (position.y + height))
			return true;
		return false;
	}

	void UIElementBase::onHoverEnterFuncDefaults()
	{

	}

	void UIElementBase::onHoverExitFuncDefaults()
	{

	}

	void UIElementBase::checkIfMouseHoverThis(glm::vec2 lastMousePosition)
	{
		if (color.a == 0.0f || isStatic) return;
		if (checkIfCollides(lastMousePosition))
		{
			if (!gotMousedHovered)
			{
				onHoverEnterFunc();
				onHoverEnterFuncDefaults();
				gotMousedHovered = true;
			}
		}
		else
		{
			if (gotMousedHovered)
			{
				onHoverExitFunc();
				onHoverExitFuncDefaults();
				gotMousedHovered = false;
			}
		}
	}

	void UIElementBase::checkForMouseClickOnThis(std::shared_ptr<InputManager> inputManager, glm::vec2 lastMousePosition)
	{
		if (color.a == 0.0f || isStatic || !checkIfCollides(lastMousePosition)) return;

		if (!inputManager->getLastLeftMouseState() && inputManager->getLeftMouseState())
			onMouseClickFunc();
		else if (inputManager->getLastLeftMouseState() && !inputManager->getLeftMouseState())
			onMouseReleaseFunc();
	}
}