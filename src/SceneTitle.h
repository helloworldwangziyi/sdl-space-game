#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "Scene.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class SceneTitle : public Scene
{
public:
    virtual void Init() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual void clean() override;
    virtual void handleEvent(SDL_Event* event) override;

private:
    Mix_Music* bgm; // 背景音乐
    float timer = 0.0f; // 计时器
};

#endif // SCENE_TITLE_H