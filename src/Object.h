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
    int maxHealth = 3; // 最大生命值
    Uint32 coolDown = 300; // 冷却时间
    Uint32 lastFireTime = 0; // 上一次发射时间
};

struct Bullet{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 800; // 子弹移动速度
    int damage = 1; // 子弹伤害
};

struct Enemy{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 100; // 敌机移动速度
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
    int speed = 300; // 子弹移动速度
    int damage = 1; // 子弹伤害
};

struct Explosion{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int currentFrame = 0; // 当前帧索引
    int totalFrames = 0; // 总帧数
    Uint32 startTime = 0; // 开始时间
    Uint32 FPS = 10; // 帧率
};


// 物品类型枚举
enum class ItemType{
    Life,
    Shield,
    Time
};

// 物品结构体
struct Item{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 200;
    int bounceCount = 3;
    ItemType type = ItemType::Life;
};


struct Backgroud{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    float offset = 0;
    int width = 0;
    int height = 0;
    int speed = 30;
};

#endif // OBJECT_H