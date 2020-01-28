#ifndef menuH
#define menuH

namespace Engine
{
	class Text;
	class UIElementBase;

	class Menu
	{
		public:
			void addText(const std::shared_ptr<Text>& text) { m_texts.push_back(text); }
			void addUIElement(const std::shared_ptr<UIElementBase>& uiElement) { m_elements.push_back(uiElement); }
			std::vector<std::shared_ptr<UIElementBase>>* getElements() { return &m_elements; }
			std::vector<std::shared_ptr<Text>>* getTexts() { return &m_texts; }
		private:
			std::vector<std::shared_ptr<UIElementBase>> m_elements;
			std::vector<std::shared_ptr<Text>> m_texts;
	};
}
#endif