// SceneEnd.h
#ifndef SCENE_END_H
#define SCENE_END_H

#include "Scene.h"
#include <string>

class SceneEnd : public Scene{
public:
    virtual void Init();
    virtual void Update(float deltaTime);
    virtual void Render();
    virtual void clean();
    virtual void handleEvent(SDL_Event* event);

private:
    bool isTyping = true;
    std::string name = "";

    void renderPhase1();
    void renderPhase2();
};

#endif // SCENE_END_H