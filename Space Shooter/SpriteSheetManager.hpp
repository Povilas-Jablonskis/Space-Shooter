#ifndef spriteSheetManagerH
#define spriteSheetManagerH

#include <string>
#include <memory>
#include <unordered_map>

class SpriteSheet;

class SpriteSheetManager
{
public:
	void loadSpriteSheetsFromConfig();
	std::shared_ptr<SpriteSheet> getSpriteSheet(const std::string&);
private:
	void loadSpriteSheet(const std::string&, const std::shared_ptr<SpriteSheet>&);
	std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> m_spriteSheets;
};
#endif