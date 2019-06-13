#include "UIElementBase.h"

namespace Engine
{
	UIElementBase::UIElementBase(glm::vec4 _color, glm::vec2 _positionPerc, std::shared_ptr<UIInputComponent> input) :
		RenderObject(0.0f, 0.0f, glm::vec2(0.0f, 0.0f), _color), gotMousedHovered(false), active(true), positionPercents(_positionPerc), inputComponent(input)
	{
		setScale(1.0f);
		setOriginalWidth(0.0f);
		setOriginalHeight(0.0f);

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


	void UIElementBase::update(float dt, std::shared_ptr<InputManager> inputManager)
	{
		if (!isActive()) return;

		updateAnimation(dt);
		fixPosition();
		inputComponent->update(this, inputManager);
	}

	void UIElementBase::fixPosition()
	{
		float windowWidth = (float)glutGet(GLUT_WINDOW_WIDTH);
		float windowHeight = (float)glutGet(GLUT_WINDOW_HEIGHT);

		if (positionPercents == glm::vec2(0.0f, 0.0f))
		{
			setPosition(glm::vec2(0.0f, 0.0f));
			setWidth(windowWidth);
			setHeight(windowHeight);
			return;
		}

		setPosition(glm::vec2(windowWidth * (positionPercents.x / 100.0f), windowHeight * (positionPercents.y / 100.0f)));

		setWidth((getOriginalWidth() * windowWidth) / (float)glutGet(GLUT_INIT_WINDOW_WIDTH));
		setHeight((getOriginalHeight() * windowHeight) / (float)glutGet(GLUT_INIT_WINDOW_HEIGHT));
	}

	bool UIElementBase::checkIfCollides(glm::vec2 colCoordinates)
	{
		// Collision x-axis?
		bool collisionX = getPosition().x + getWidth() >= colCoordinates.x &&
			colCoordinates.x >= getPosition().x;
		// Collision y-axis?
		bool collisionY = getPosition().y + getHeight() >= colCoordinates.y &&
			colCoordinates.y >= getPosition().y;
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
}