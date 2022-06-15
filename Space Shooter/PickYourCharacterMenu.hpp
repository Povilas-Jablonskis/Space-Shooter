#ifndef pickYourCharacterMenu_hpp
#define pickYourCharacterMenu_hpp

#include "Scene.hpp"
#include "UIElementBase.hpp"

class SpriteSheetManager;

class PickYourCharacterMenu : public Scene
{
public:
    PickYourCharacterMenu(std::shared_ptr<SceneStateMachine>, std::shared_ptr<SpriteSheetManager>, std::shared_ptr<InputManager>, irrklang::ISoundEngine*);

    void loadPlayerModels(const std::shared_ptr<SpriteSheetManager>&);

    void onCreate() override;
    void onDestroy() override;

    void onActivate() override;

    void processInput() override;

    void draw(const std::shared_ptr<Renderer>&, const float) override;
private:
    std::vector<std::shared_ptr<UIElementBase>> m_elements;
    std::vector<std::shared_ptr<Text>> m_texts;
    int m_characterSelectionIndex{ 0 };
    std::vector<std::shared_ptr<Animation>> m_playerModels;
    std::shared_ptr<InputManager> m_inputManager{ nullptr };
    std::shared_ptr<SpriteSheetManager> m_spriteSheetManager{ nullptr };
    irrklang::ISoundEngine* m_soundEngine{ nullptr };
    std::shared_ptr<SceneStateMachine> m_sceneStateMachine{ nullptr };
};

#endif /* SceneSplashScreen_hpp */