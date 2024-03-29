#ifndef pickYourCharacterMenu_hpp
#define pickYourCharacterMenu_hpp

#include "Scene.hpp"
#include "ObjectCollection.hpp"
#include "SceneStateMachine.hpp"
#include "SharedContext.hpp"

class PickYourCharacterMenu final : public Scene
{
public:
	PickYourCharacterMenu(SceneStateMachine& sceneStateMachine, SharedContext& context);

	void loadPlayerModels();

	void onCreate() override;
	void onDestroy() override;

	void onActivate() override;

	void processInput() override;

	void draw(float dt) override;

private:
	std::vector<std::shared_ptr<Text>> m_texts;
	int m_characterSelectionIndex{};
	std::vector<std::string> m_playerModels;

	ObjectCollection m_objects;
	SharedContext& m_context;
	SceneStateMachine& m_sceneStateMachine;
};

#endif
