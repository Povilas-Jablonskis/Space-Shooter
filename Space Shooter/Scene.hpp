#ifndef scene_hpp
#define scene_hpp

#include <irrKlang/irrKlang.h>
#include <vector>
#include <memory>
#include <glew/glew.h>
#include <freeglut/freeglut.h>

class Renderer;

class Scene
{
public:
    virtual void onCreate() = 0; // Called when scene initially created. Called once per scene.
    virtual void onDestroy() = 0;

    virtual void onActivate() {}; // Called whenever a scene is activated.
    virtual void onDeactivate() {}; // Called whenever a scene is deactivated.

    virtual void processInput() {};
    virtual void draw(float) {};
};

#endif