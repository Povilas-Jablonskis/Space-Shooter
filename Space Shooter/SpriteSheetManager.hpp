#ifndef spriteSheetManagerH
#define spriteSheetManagerH

#include <string>
#include <memory>
#include <vector>

namespace Engine
{
	class SpriteSheet;

	typedef std::pair<std::string, std::shared_ptr<SpriteSheet>> spriteSheet;

	class SpriteSheetManager
	{
		public:
			void loadSpriteSheetsFromConfig();
			std::shared_ptr<SpriteSheet> getSpriteSheet(const std::string&);
		private:
			std::vector<spriteSheet>* getSpriteSheets() { return &m_spriteSheets; }

			void loadSpriteSheet(const std::string&, const std::shared_ptr<SpriteSheet>&);
			std::vector<spriteSheet> m_spriteSheets;
	};
}
#endif