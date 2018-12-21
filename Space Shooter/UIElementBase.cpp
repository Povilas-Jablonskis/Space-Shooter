#include "UIElementBase.h"

namespace Engine
{
	UIElementBase::UIElementBase(float _width, float _height, glm::vec2 _position, glm::vec4 _color, glm::vec2 _positionPerc) :
		RenderObject(_width, _height, _position, _color), gotMousedHovered(false), isStatic(false), positionPercents(_positionPerc), originalWidth(width), originalHeigth(height)
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

		if (_positionPerc != glm::vec2(0.0f, 0.0f))
			fixPosition();
	}

	void UIElementBase::update(float dt)
	{
		if (color.a == 0.0f) return;

		updateAnimation(dt);
	}

	void UIElementBase::fixPosition()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

		if (positionPercents == glm::vec2(0.0f, 0.0f))
		{
			width = temPos.x;
			height = temPos.y;
			return;
		}
		position.x = temPos.x * (positionPercents.x / 100.0f);
		position.y = temPos.y * (positionPercents.y / 100.0f);

		width = originalWidth * (temPos.x / (float)glutGet(GLUT_INIT_WINDOW_WIDTH));
		height = originalHeigth * (temPos.y / (float)(glutGet(GLUT_INIT_WINDOW_HEIGHT)));
	}

	bool UIElementBase::checkIfCollides(glm::vec2 colCoordinates)
	{
		// Collision x-axis?
		bool collisionX = position.x + width >= colCoordinates.x &&
			colCoordinates.x >= position.x;
		// Collision y-axis?
		bool collisionY = position.y + height >= colCoordinates.y &&
			colCoordinates.y >= position.y;
		// Collision only if on both axes
		if (collisionX && collisionY)
			return true;
		return false;
	}

	void UIElementBase::onHoverEnterFuncDefaults()
	{

	}

	void UIElementBase::onHoverExitFuncDefaults()
	{

	}

	bool UIElementBase::checkIfMouseHoverThis(glm::vec2 lastMousePosition)
	{
		if (color.a == 0.0f) return false;
		
		if (checkIfCollides(lastMousePosition))
		{
			if (!gotMousedHovered)
			{
				if(!isStatic) onHoverEnterFuncDefaults();
				onHoverEnterFunc();
				gotMousedHovered = true;
				return true;
			}
		}
		else
		{
			if (gotMousedHovered)
			{
				if (!isStatic) onHoverExitFuncDefaults();
				onHoverExitFunc();
				gotMousedHovered = false;
				return true;
			}
		}
		return false;
	}

	bool UIElementBase::checkForMouseClickOnThis(bool leftMouseState, bool lastLeftMouseState, glm::vec2 lastMousePosition)
	{
		if (color.a == 0.0f || !checkIfCollides(lastMousePosition)) return false;

		if (!lastLeftMouseState && leftMouseState)
		{
			onMouseClickFunc();
			return true;
		}
		else if (lastLeftMouseState && !leftMouseState)
		{
			onMouseReleaseFunc();
			return true;
		}

		return false;
	}
}