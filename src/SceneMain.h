//
// Created by Administrator on 25-5-11.
//
#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"
#include <list>

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

    void shootBullet(); // 发射子弹

    void updateBullet(float deltaTime); // 更新子弹

    void renderBullet(); // 渲染子弹

private:
    Player player;
    Game &game;

    // 子弹
    Bullet bullet;

    // 子弹列表
    std::list<Bullet*> bullets;

};

#endif // SCENE_MAIN_H