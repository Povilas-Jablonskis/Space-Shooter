#ifndef controlsMenu_hpp
#define controlsMenu_hpp

#include "Scene.hpp"
#include "SceneStateMachine.hpp"
#include "SharedContext.hpp"
#include "Text.hpp"

class ControlsMenu final : public Scene
{
public:
	ControlsMenu(SceneStateMachine&, SharedContext&);

	void onCreate() override;
	void onDestroy() override;

	void onActivate() override;

	void processInput() override;

	void draw(float) override;

private:
	std::vector<std::shared_ptr<Text>> m_texts;
	SharedContext& m_context;
	SceneStateMachine& m_sceneStateMachine;
	std::unordered_map<BindableAction, std::shared_ptr<Text>> m_keybindsTexts;
	std::unique_ptr<BindableAction> m_currentlyEditedKeybind{};
};

#endif
