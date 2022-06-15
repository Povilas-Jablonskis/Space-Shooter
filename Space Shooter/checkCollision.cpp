#include "checkCollision.hpp"
#include "InputManager.hpp"
#include "UIElementBase.hpp"
#include "Text.hpp"

void checkCollision(UIElementBase* uiElementBase, const std::shared_ptr<InputManager>& inputManager)
{
	if (!uiElementBase->isActive())
	{
		return;
	}

	// Collision x-axis?
	const auto collisionX = uiElementBase->getPosition().x + uiElementBase->getWidth() >= inputManager->getLastMousePosition().x &&
		inputManager->getLastMousePosition().x >= uiElementBase->getPosition().x;
	// Collision y-axis?
	const auto collisionY = uiElementBase->getPosition().y + uiElementBase->getHeight() >= inputManager->getLastMousePosition().y &&
		inputManager->getLastMousePosition().y >= uiElementBase->getPosition().y;

	if (collisionX && collisionY)
	{
		if (!uiElementBase->isClickedByMouse())
		{
			if (!inputManager->getLastLeftMouseState() && inputManager->getLeftMouseState())
			{
				uiElementBase->onMouseClickFunc();
				uiElementBase->setMousedClicked(true);
			}
		}
		else
		{
			if (inputManager->getLastLeftMouseState() && !inputManager->getLeftMouseState())
			{
				uiElementBase->onMouseReleaseFunc();
				uiElementBase->setMousedClicked(false);
			}
		}

		if (!uiElementBase->isHoveredByMouse())
		{
			uiElementBase->onHoverEnterFuncDefaults();
			uiElementBase->onHoverEnterFunc();
			uiElementBase->setMousedHovered(true);
		}
	}
	else
	{
		if (uiElementBase->isHoveredByMouse())
		{
			uiElementBase->onHoverExitFuncDefaults();
			uiElementBase->onHoverExitFunc();
			uiElementBase->setMousedHovered(false);
		}
	}
}


void checkCollision(Text* text, const std::shared_ptr<InputManager>& inputManager)
{
	if (!text->isActive())
	{
		return;
	}

	const auto collision = text->getBoundingBox().x >= text->getBoundingBox().x && inputManager->getLastMousePosition().x <= text->getBoundingBox().y && inputManager->getLastMousePosition().y <= text->getBoundingBox().z && inputManager->getLastMousePosition().y >= text->getBoundingBox().a;

	if (collision)
	{
		if (!text->isClickedByMouse())
		{
			if (!inputManager->getLastLeftMouseState() && inputManager->getLeftMouseState())
			{
				text->onMouseClickFunc();
				text->setMousedClicked(true);
			}
		}
		else
		{
			if (inputManager->getLastLeftMouseState() && !inputManager->getLeftMouseState())
			{
				text->onMouseReleaseFunc();
				text->setMousedClicked(false);
			}
		}

		if (!text->isHoveredByMouse())
		{
			text->onHoverEnterFuncDefaults();
			text->onHoverEnterFunc();
			text->setMousedHovered(true);
		}
	}
	else
	{
		if (text->isHoveredByMouse())
		{
			text->onHoverExitFuncDefaults();
			text->onHoverExitFunc();
			text->setMousedHovered(false);
		}
	}
}