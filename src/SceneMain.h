//
// Created by Administrator on 25-5-11.
//
#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"

class Game;

class SceneMain : public Scene {
public:
    SceneMain();
    ~SceneMain();

    void Update(float deltaTime) override;
    void Render() override;
    void handleEvent(SDL_Event* event) override;
    void Init() override;
    void clean() override;

    void keyboardControl(float deltaTime);

private:
    Player player;
    Game &game;

};

#endif // SCENE_MAIN_H