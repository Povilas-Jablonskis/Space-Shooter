#ifndef pickYourCharacterMenu_hpp
#define pickYourCharacterMenu_hpp

#include "Scene.hpp"
#include "UIElementBase.hpp"
#include "ResourceAllocator.hpp"

class SpriteSheetManager;
class Texture;

class PickYourCharacterMenu : public Scene
{
public:
    PickYourCharacterMenu(std::shared_ptr<SceneStateMachine>, std::shared_ptr<SpriteSheetManager>, std::shared_ptr<InputManager>, irrklang::ISoundEngine*, ResourceAllocator<Texture>&);

    void loadPlayerModels(const std::shared_ptr<SpriteSheetManager>&);

    void onCreate() override;
    void onDestroy() override;

    void onActivate() override;

    void processInput() override;

    void draw(const std::shared_ptr<Renderer>&, const float) override;
private:
    std::vector<std::shared_ptr<UIElementBase>> m_elements;
    std::vector<std::shared_ptr<Text>> m_texts;
    int m_characterSelectionIndex{};
    std::vector<std::shared_ptr<Animation>> m_playerModels;

    ResourceAllocator<Texture>& m_textureAllocator;
    std::shared_ptr<InputManager> m_inputManager{};
    std::shared_ptr<SpriteSheetManager> m_spriteSheetManager{};
    irrklang::ISoundEngine* m_soundEngine{};
    std::shared_ptr<SceneStateMachine> m_sceneStateMachine{};
};

#endif