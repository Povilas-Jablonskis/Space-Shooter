#include "UIElementBase.hpp"
#include "UIInputComponent.hpp"

#include <glew/glew.h>
#include <freeglut/freeglut.h>

namespace Engine
{
	UIElementBase::UIElementBase(const glm::vec4& color, const glm::vec2& positionPerc) : RenderObject(0.0f, 0.0f, glm::vec2(0.0f, 0.0f), color), m_positionPercents(positionPerc)
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


	void UIElementBase::update(const float dt, const std::shared_ptr<InputManager>& inputManager)
	{
		if (!isActive())
		{
			return;
		}

		updateAnimation(dt);
		fixPosition();
		updateInput(this, inputManager);
	}

	void UIElementBase::fixPosition()
	{
		const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
		const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

		if (getPositionPercents() == glm::vec2(0.0f, 0.0f))
		{
			setPosition(glm::vec2(0.0f, 0.0f));
			setWidth(windowWidth);
			setHeight(windowHeight);
			return;
		}

		setPosition(glm::vec2(windowWidth * (getPositionPercents().x / 100.0f), windowHeight * (getPositionPercents().y / 100.0f)));

		setWidth(getOriginalWidth() * windowWidth / static_cast<float>(glutGet(GLUT_INIT_WINDOW_WIDTH)));
		setHeight(getOriginalHeight() * windowHeight / static_cast<float>(glutGet(GLUT_INIT_WINDOW_HEIGHT)));
	}

	bool UIElementBase::checkIfCollides(const glm::vec2& colCoordinates) const
	{
		// Collision x-axis?
		const auto collisionX = getPosition().x + getWidth() >= colCoordinates.x &&
			colCoordinates.x >= getPosition().x;
		// Collision y-axis?
		const auto collisionY = getPosition().y + getHeight() >= colCoordinates.y &&
			colCoordinates.y >= getPosition().y;
		// Collision only if on both axes
		return collisionX && collisionY;
	}

	void UIElementBase::onHoverEnterFuncDefaults()
	{

	}

	void UIElementBase::onHoverExitFuncDefaults()
	{

	}
}