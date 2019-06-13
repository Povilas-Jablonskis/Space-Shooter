#include "UIInputComponent.h"
#include "UIElementBase.h"

namespace Engine
{
	void UIInputComponent::update(UIElementBase* uiElement, std::shared_ptr<InputManager> inputManager)
	{
		if (!uiElement->isActive()) return;

		if (uiElement->checkIfCollides(inputManager->getLastMousePosition()))
		{
			if (!inputManager->getLastLeftMouseState() && inputManager->getLeftMouseState())
			{
				uiElement->onMouseClickFunc();
			}
			else if (inputManager->getLastLeftMouseState() && !inputManager->getLeftMouseState())
			{
				uiElement->onMouseReleaseFunc();
			}

			if (!uiElement->isMousedHovered())
			{
				uiElement->onHoverEnterFuncDefaults();
				uiElement->onHoverEnterFunc();
				uiElement->setMousedHovered(true);
			}
		}
		else
		{
			if (uiElement->isMousedHovered())
			{
				uiElement->onHoverExitFuncDefaults();
				uiElement->onHoverExitFunc();
				uiElement->setMousedHovered(false);
			}
		}
	}
}