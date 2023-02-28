#ifndef controlsMenu_hpp
#define controlsMenu_hpp

#include <string>

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

	void savePlayerConfig() const;
	static std::string sVirtualKeyCodeToString(int);

private:
	std::vector<std::shared_ptr<Text>> m_texts;
	SharedContext& m_context;
	SceneStateMachine& m_sceneStateMachine;
};

#endif
