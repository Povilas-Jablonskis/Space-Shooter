#ifndef pickYourCharacterMenu_hpp
#define pickYourCharacterMenu_hpp

#include "Scene.hpp"
#include "ObjectCollection.hpp"
#include "SceneStateMachine.hpp"
#include "SharedContext.hpp"

class Text;

class PickYourCharacterMenu : public Scene
{
public:
    PickYourCharacterMenu(SceneStateMachine&, SharedContext&);

    void loadPlayerModels();

    void onCreate() override;
    void onDestroy() override;

    void onActivate() override;

    void processInput() override;

    void draw(float) override;
private:
    std::vector<std::shared_ptr<Text>> m_texts;
    int m_characterSelectionIndex{};
    std::vector<std::string> m_playerModels;

    ObjectCollection m_objects;
    SharedContext& m_context;
    SceneStateMachine& m_sceneStateMachine;
};

#endif