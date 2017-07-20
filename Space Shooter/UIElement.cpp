#include "UIElement.h"
#include <algorithm>

namespace Engine
{
	UIElement::UIElement(int _width, int _height, glm::vec2 _position, glm::vec4 _color, std::string _parent, glm::vec2 _positionPerc)
		: UIElementBase(_width, _height, _position, _color, _positionPerc), parentMenu(_parent)
	{

	}

	UIElement::~UIElement()
	{
		texts.clear();
		elements.clear();
	}

	void UIElement::update(InputManager* inputManager, float dt)
	{
		UIElementBase::update(inputManager, dt);
		for (auto text : texts)
		{
			text->update(inputManager);
		}
		for (auto element : elements)
		{
			element->update(inputManager, dt);
		}
	}

	void UIElement::draw(GLuint program, GLuint program2, GLuint vao, GLuint vbo, GLuint _texture)
	{
		UIElementBase::draw(program, vao);
		for (auto element : elements)
		{
			element.get()->draw(program, program2, vao, vbo, _texture);
		}

		glUseProgram(program2);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _texture);

			/* We require 1 byte alignment when uploading texture data */
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			/* Clamping to edges is important to prevent artifacts when scaling */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			/* Linear filtering usually looks best for text */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
				for (auto text : texts)
				{
					text.get()->draw(program2, vbo, _texture);
				}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDisable(GL_TEXTURE_2D);
		glUseProgram(0);
	}

	void UIElement::hideAllElements()
	{
		for (auto text : texts)
		{
			text->changeColor(0.0f, 3);
		}
		for (auto element : elements)
		{
			element->hideAllElements();
		}
	}

	void UIElement::showAllElements()
	{
		for (auto text : texts)
		{
			text->changeColor(1.0f, 3);
		}
		for (auto element : elements)
		{
			element->showAllElements();
		}
	}

	void UIElement::onMouseClickDefaults(InputManager* inputManager)
	{
		for (auto text : texts)
		{
			text->onMouseClickDefaults(inputManager);
		}
		for (auto element : elements)
		{
			element->onMouseClickDefaults(inputManager);
		}

		UIElementBase::onMouseClickDefaults(inputManager);
	}

	void UIElement::onMouseReleaseFuncDefaults(InputManager* inputManager)
	{
		for (auto text : texts)
		{
			text->onMouseReleaseFuncDefaults(inputManager);
		}
		for (auto element : elements)
		{
			element->onMouseReleaseFuncDefaults(inputManager);
		}

		UIElementBase::onMouseReleaseFuncDefaults(inputManager);
	}

	void UIElement::onHoverEnterFuncDefaults()
	{

	}

	void UIElement::onHoverExitFuncDefaults()
	{

	}

	void UIElement::fixPosition(UIElementBase* _parent)
	{
		UIElementBase::fixPosition(_parent);

		for (auto text : texts)
		{
			text->fixPosition(this);
		}
		for (auto element : elements)
		{
			element->fixPosition(this);
		}
	}
}