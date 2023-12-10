#include "InputManager.hpp"

#include <system_error>
#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <magic_enum/magic_enum.hpp>
#include <fstream>
#include "rapidxml/rapidxml_ext.hpp"
#include <sstream>
#include <algorithm>

#include "Configs.hpp"

bool InputManager::isLeftMouseClicked() const
{
	return m_leftMouseClick;
}

bool InputManager::isRightMouseClicked() const
{
	return m_rightMouseClick;
}

void InputManager::setLeftMouseClicked(const bool leftMouseClick)
{
	m_leftMouseClick = leftMouseClick;
}

void InputManager::setRightMouseClicked(const bool rightMouseClick)
{
	m_rightMouseClick = rightMouseClick;
}

bool InputManager::isLastLeftMouseStateClicked() const
{
	return m_lastLeftMouseClick;
}

bool InputManager::isLastRightMouseStateClicked() const
{
	return m_lastRightMouseClick;
}

void InputManager::setLastLeftMouseClicked(const bool lastLeftMouseClick)
{
	m_lastLeftMouseClick = lastLeftMouseClick;
}

void InputManager::setLastRightMouseClicked(const bool lastRightMouseClick)
{
	m_lastRightMouseClick = lastRightMouseClick;
}

void InputManager::setLastMousePosition(const glm::vec2& lastMousePosition)
{
	m_lastMousePosition = lastMousePosition;
}

const glm::vec2& InputManager::getLastMousePosition() const
{
	return m_lastMousePosition;
}

bool InputManager::isKeyActive(const short key) const
{
	try
	{
		return m_keysStates.at(key);
	}
	catch (const std::exception&)
	{
		return false;
	}
}

void InputManager::changeKeyState(const short key, bool boolean)
{
	m_keysStates.insert_or_assign(key, boolean);
}

std::unordered_map<short, bool>& InputManager::getKeysStates()
{
	return m_keysStates;
}

void InputManager::motionFunc(const int x, const int y)
{
	auto lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));
	lastMousePosition.y = abs(lastMousePosition.y);
	setLastMousePosition(lastMousePosition);
}

void InputManager::processMouseClick(const int button, const int state, const int x, const int y)
{
	auto lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));
	lastMousePosition.y = abs(lastMousePosition.y);

	if (button == GLUT_LEFT_BUTTON)
	{
		setLastLeftMouseClicked(isLeftMouseClicked());
		setLeftMouseClicked(state == GLUT_DOWN);
		setLastMousePosition(lastMousePosition);
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		setLastRightMouseClicked(isRightMouseClicked());
		setRightMouseClicked(state == GLUT_DOWN);
		setLastMousePosition(lastMousePosition);
	}
}

void InputManager::keyboardInput(const unsigned char c)
{
	const auto key = VkKeyScan(static_cast<char>(c));
	if (! isKeyActive(key))
	{
		changeKeyState(key, true);
	}
}

void InputManager::keyboardInputUp(const unsigned char c, int, int)
{
	const auto key = VkKeyScan(static_cast<char>(c));
	if (isKeyActive(key))
	{
		changeKeyState(key, false);
	}
}

void InputManager::specialKeyInput(const int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		{
			if (isKeyActive(VK_UP))
			{
				changeKeyState(VK_UP, false);
			}
			break;
		}
	case GLUT_KEY_DOWN:
		{
			if (isKeyActive(VK_DOWN))
			{
				changeKeyState(VK_DOWN, false);
			}
			break;
		}
	case GLUT_KEY_LEFT:
		{
			if (isKeyActive(VK_LEFT))
			{
				changeKeyState(VK_LEFT, false);
			}
			break;
		}
	case GLUT_KEY_RIGHT:
		{
			if (isKeyActive(VK_RIGHT))
			{
				changeKeyState(VK_RIGHT, false);
			}
			break;
		}
	default: ;
	}
}

void InputManager::specialKeyInputUp(const int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		{
			if (! isKeyActive(VK_UP))
			{
				changeKeyState(VK_UP, true);
			}
			break;
		}
	case GLUT_KEY_DOWN:
		{
			if (! isKeyActive(VK_DOWN))
			{
				changeKeyState(VK_DOWN, true);
			}
			break;
		}
	case GLUT_KEY_LEFT:
		{
			if (! isKeyActive(VK_LEFT))
			{
				changeKeyState(VK_LEFT, true);
			}
			break;
		}
	case GLUT_KEY_RIGHT:
		{
			if (! isKeyActive(VK_RIGHT))
			{
				changeKeyState(VK_RIGHT, true);
			}
			break;
		}
	default: ;
	}
}

void InputManager::clearEverything()
{
	m_lastMousePosition = glm::vec2(0.0f, 0.0f);
	m_lastLeftMouseClick = false;
	m_leftMouseClick = false;
	m_lastRightMouseClick = false;
	m_rightMouseClick = false;
	m_keysStates.clear();
}

void InputManager::checkInteraction(const std::shared_ptr<Text>& text) const
{
	if (! text->isActive())
	{
		return;
	}

	auto mouseIntersectsText = getLastMousePosition().x >= text->getBoundingBox().x && getLastMousePosition().x <=
		text->getBoundingBox().y && getLastMousePosition().y <= text->getBoundingBox().z &&
		getLastMousePosition().y >= text->getBoundingBox().a;

	if (!isLastLeftMouseStateClicked() && isLeftMouseClicked())
	{
		if (mouseIntersectsText)
		{
			if (!text->isClickedByMouse()) {
				text->onMouseClickFunc();
				text->setMousedClicked(true);
			}
		}
		else {
			text->setMousedClicked(false);
		}
	} 
	else
	{
		if (mouseIntersectsText)
		{
			if (text->isClickedByMouse()) {
				text->onMouseReleaseFunc();
			}
		}
		text->setMousedClicked(false);
	}

	if (!text->isHoveredByMouse())
	{
		if (mouseIntersectsText)
		{
			text->onHoverEnterFuncDefaults();
			text->onHoverEnterFunc();
			text->setMousedHovered(true);
		}
		else {
			text->setMousedHovered(false);
		}
	}
	else
	{
		if (!mouseIntersectsText)
		{
			text->onHoverExitFuncDefaults();
			text->onHoverExitFunc();
			text->setMousedHovered(false);
		}
	}
}

short InputManager::getKeybind(const BindableAction bindableAction) const
{
	try
	{
		return m_keybinds.at(bindableAction);
	}
	catch (const std::exception&)
	{
		return 0;
	}
}

void InputManager::setKeybindCharacter(BindableAction key, short value)
{
	m_keybinds[key] = value;
}

bool InputManager::keybindingsContainKey(const short keyBind) const
{
	return std::any_of(m_keybinds.begin(), m_keybinds.end(), [=](auto& pair)
		{
			return pair.second == keyBind;
		});
}

void InputManager::loadKeybinds()
{
	const auto keybindsFileDoc = new rapidxml::xml_document<>();
	// Read the xml file into a vector
	std::ifstream keybindsFile(Configs::KEYBINDS_SETTINGS_PATH);
	std::vector keybindsFileBuffer((std::istreambuf_iterator(keybindsFile)), std::istreambuf_iterator<char>());
	keybindsFileBuffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	keybindsFileDoc->parse<0>(keybindsFileBuffer.data());

	for (auto keybindsNode = keybindsFileDoc->first_node("Keybinds"); keybindsNode; keybindsNode =
	     keybindsNode->
	     next_sibling("Keybinds"))
	{
		for (auto keybindNode = keybindsNode->first_node("Keybind"); keybindNode; keybindNode =
		     keybindNode->
		     next_sibling("Keybind"))
		{
			const auto keyNode = magic_enum::enum_cast<BindableAction>(keybindNode->first_attribute("key")->value());

			if (!keyNode.has_value()) continue;

			short valueNode;

			std::stringstream ss(keybindNode->first_attribute("value")->value());
			ss >> valueNode;

			m_keybinds[keyNode.value()] = valueNode;
		}
	}

	keybindsFile.close();
	keybindsFileDoc->clear();
	delete keybindsFileDoc;
}

void InputManager::saveKeybinds() const
{
	const auto keybindsFileDoc = new rapidxml::xml_document<>();

	const auto keybindsNode = keybindsFileDoc->allocate_node(rapidxml::node_type::node_element, "Keybinds");

	for (const auto& keybind : m_keybinds)
	{
		const auto& [key, value] = keybind;

		const auto keybindNode = keybindsFileDoc->allocate_node(rapidxml::node_type::node_element, "Keybind");

		const auto bindableActionValue = std::string(magic_enum::enum_name(key));
		const auto keybindNodeAttributeKey = keybindsFileDoc->allocate_string(bindableActionValue.c_str());
		keybindNode->append_attribute(keybindsFileDoc->allocate_attribute("key", keybindNodeAttributeKey));

		const auto keybindNodeAttributeValue = keybindsFileDoc->allocate_string(
			std::to_string(value).c_str());
		keybindNode->append_attribute(keybindsFileDoc->allocate_attribute("value", keybindNodeAttributeValue));

		keybindsNode->append_node(keybindNode);
	}

	keybindsFileDoc->append_node(keybindsNode);

	std::ofstream keybindsFileStream(Configs::KEYBINDS_SETTINGS_PATH);
	keybindsFileStream << *keybindsFileDoc;
	keybindsFileStream.close();
	keybindsFileDoc->clear();
	delete keybindsFileDoc;
}

std::string InputManager::virtualKeyCodeToString(const int virtualKey) const
{
	const LONG scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

	TCHAR szName[128];
	int result = 0;
	switch (virtualKey)
	{
	case VK_LEFT:
	case VK_UP:
	case VK_RIGHT:
	case VK_DOWN:
	case VK_RCONTROL:
	case VK_RMENU:
	case VK_LWIN:
	case VK_RWIN:
	case VK_APPS:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_END:
	case VK_HOME:
	case VK_INSERT:
	case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		KF_EXTENDED;
		break;
	default:
		result = GetKeyNameText(scanCode << 16, szName, 128);
		break;
	}
	if (result == 0)
	{
		const auto dwErrVal = static_cast<int>(GetLastError());
		throw std::system_error(std::error_code(dwErrVal, std::system_category()),
			"WinAPI Error occurred.");
	}
	return szName;
}
