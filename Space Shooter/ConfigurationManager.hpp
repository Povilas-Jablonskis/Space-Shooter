#ifndef configurationManagerH
#define configurationManagerH

#include <memory>

namespace Engine
{
	class Font;

	class ConfigurationManager
	{
		public:
			ConfigurationManager();
			inline const std::shared_ptr<Font>& getInterfaceFont() const { return m_interfaceFont; }
		private:
			inline void setInterfaceFont(const std::shared_ptr<Font>& t_font) { m_interfaceFont = t_font; }

			std::shared_ptr<Font> m_interfaceFont;
	};
}
#endif