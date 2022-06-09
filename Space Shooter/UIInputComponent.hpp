#ifndef uiinputComponentH
#define uiinputComponentH

#include "InputManager.hpp"

class UIInputComponent
{
public:
	template <typename T>
	void updateInput(T* object, const std::shared_ptr<InputManager>& inputManager)
	{
		if (!object->isActive())
		{
			return;
		}

		if (object->checkIfCollides(inputManager->getLastMousePosition()))
		{
			if (!object->isClickedByMouse())
			{
				if (!inputManager->getLastLeftMouseState() && inputManager->getLeftMouseState())
				{
					object->onMouseClickFunc();
					object->setMousedClicked(true);
				}
			}
			else
			{
				if (inputManager->getLastLeftMouseState() && !inputManager->getLeftMouseState())
				{
					object->onMouseReleaseFunc();
					object->setMousedClicked(false);
				}
			}

			if (!object->isHoveredByMouse())
			{
				object->onHoverEnterFuncDefaults();
				object->onHoverEnterFunc();
				object->setMousedHovered(true);
			}
		}
		else
		{
			if (object->isHoveredByMouse())
			{
				object->onHoverExitFuncDefaults();
				object->onHoverExitFunc();
				object->setMousedHovered(false);
			}
		}
	}
};
#endif