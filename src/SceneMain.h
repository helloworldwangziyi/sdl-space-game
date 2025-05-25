//
// Created by Administrator on 25-5-11.
//
#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"
#include <list>
#include <random>

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

    void updatePlayer(float deltaTime); // 更新玩家

    void shootBullet(); // 发射子弹
    void updateBullet(float deltaTime); // 更新子弹
    void renderBullet(); // 渲染子弹

    void spawnEnemy(); // 生成敌机
    void updateEnemy(float deltaTime); // 更新敌机的位置
    void renderEnemy(); // 渲染敌机

    void renderEnemyBullet(); // 渲染敌机子弹
    void updateEnemyBullet(float deltaTime); // 更新敌机子弹
    void EnemyShootBullet(Enemy* enemy); // 敌机发射子弹
    SDL_FPoint getDirection(Enemy* enemy); // 计算子弹方向

    void enemyExplode(Enemy* enemy);      // 敌机爆炸效果


private:
    Player player;
    Game &game;

    // 子弹
    Bullet bullet;
    // 子弹列表
    std::list<Bullet*> bullets;

    std::mt19937 gen; // 随机数生成器
    std::uniform_real_distribution<float> dis; // 均匀分布

    // 敌机
    Enemy enemy;
    // 敌机列表
    std::list<Enemy*> enemies;

    // 敌机子弹
    EnemyBullet enemyBullet;
    // 敌机子弹列表
    std::list<EnemyBullet*> enemyBullets;

    bool isDead = false; // 玩家是否死亡

};

#endif // SCENE_MAIN_H