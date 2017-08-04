#include "UIElementBase.h"

namespace Engine
{
	UIElementBase::UIElementBase(int _width, int _height, glm::vec2 _position, glm::vec4 _color, glm::vec2 _positionPerc) :
		RenderObject(_width, _height, _position, _color), gotMousedHovered(false), isStatic(false), positionPercents(_positionPerc)
	{
		initFuncs();
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