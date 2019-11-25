#ifndef menuH
#define menuH

namespace Engine
{
	class Text;
	class UIElementBase;

	class Menu
	{
		public:
			inline void addText(const std::shared_ptr<Text>& text) { m_texts.push_back(text); }
			inline void addUIElement(const std::shared_ptr<UIElementBase>& uiElement) { m_elements.push_back(uiElement); }
			inline std::vector<std::shared_ptr<UIElementBase>>* getElements() { return &m_elements; }
			inline std::vector<std::shared_ptr<Text>>* getTexts() { return &m_texts; }
		private:
			std::vector<std::shared_ptr<UIElementBase>> m_elements;
			std::vector<std::shared_ptr<Text>> m_texts;
	};
}
#endif