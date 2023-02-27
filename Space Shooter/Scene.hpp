#ifndef scene_hpp
#define scene_hpp

class Renderer;

class Scene
{
public:
	Scene() = default;
	Scene(const Scene&) = default;
	Scene(Scene&&) = default;
	Scene& operator=(const Scene&) = default;
	Scene& operator=(Scene&&) = default;
	virtual ~Scene() = default;

	virtual void onCreate() = 0; // Called when scene initially created. Called once per scene.
	virtual void onDestroy() = 0;

	virtual void onActivate()
	{
	} // Called whenever a scene is activated.
	virtual void onDeactivate()
	{
	} // Called whenever a scene is deactivated.

	virtual void processInput()
	{
	}

	virtual void draw(float)
	{
	}
};

#endif
