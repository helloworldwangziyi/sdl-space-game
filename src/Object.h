// Object.h
#ifndef OBJECT_H
#define OBJECT_H
#include <SDL.h>

struct Player{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 400;
    int currentHealth = 3; // 当前生命值
    Uint32 coolDown = 1000; // 冷却时间
    Uint32 lastFireTime = 0; // 上一次发射时间
};

struct Bullet{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 400; // 子弹移动速度
    int damage = 1; // 子弹伤害
};

struct Enemy{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 200; // 敌机移动速度
    int currentHealth = 2; // 当前生命值
    Uint32 coolDown = 3000;    // 发射冷却时间
    Uint32 lastFireTime = 0; // 上一次发射时间
};

struct EnemyBullet{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0}; // 子弹位置
    SDL_FPoint direction = {0, 0}; // 子弹方向
    int width = 0;
    int height = 0;
    int speed = 400; // 子弹移动速度
    int damage = 1; // 子弹伤害
};

#endif // OBJECT_H