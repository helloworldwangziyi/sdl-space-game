//
// Created by Administrator on 25-5-11.
//
// SceneMain.cpp
#include "SceneMain.h"
#include "Game.h"
#include <SDL_image.h>
#include <iostream>

SceneMain::SceneMain() : game(Game::getInstance())
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Update(float deltaTime)
{
    keyboardControl(deltaTime);
    updateBullet(deltaTime);
    spawnEnemy(); // 生成敌机
    updateEnemy(deltaTime); // 更新敌机
}

void SceneMain::Render()
{

    // 渲染玩家
    SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);
    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);

    // 渲染子弹
    renderBullet();

    // 渲染敌机
    renderEnemy();
}

void SceneMain::handleEvent(SDL_Event *event)
{
}

void SceneMain::Init()
{
    // 初始化
    std::random_device rd; // 获取真随机数作为种子
    gen = std::mt19937(rd()); // 用真随机数初始化随机数生成器
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f); // 均匀分布

    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/assets/image/SpaceShip.png");
    if (player.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player texture: %s", SDL_GetError());
    }
    // 
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;

    // 初始化子弹
    bullet.texture = IMG_LoadTexture(game.getRenderer(), "assets/assets/image/laser-1.png");
    SDL_QueryTexture(bullet.texture, NULL, NULL, &bullet.width, &bullet.height);
    bullet.width /= 4;
    bullet.height /= 4;

    // 初始化敌机
    enemy.texture = IMG_LoadTexture(game.getRenderer(), "assets/assets/image/insect-2.png");
    SDL_QueryTexture(enemy.texture, NULL, NULL, &enemy.width, &enemy.height);
    enemy.width /= 4;
    enemy.height /= 4; 
}

void SceneMain::clean()
{
    // 清理子弹
    for(auto &it : bullets)
    {
        if(it != nullptr)
        {
            delete it;
        }
    }
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
    }
    if(bullet.texture != nullptr)
    {
        SDL_DestroyTexture(bullet.texture);
        bullet.texture = nullptr;
    }

    // 清理敌机
    for(auto &enemy : enemies)
    {
        if(enemy != nullptr)
        {
            delete enemy;
        }
    }
    enemies.clear();
    // 清理敌机纹理
    if(enemy.texture != nullptr)
    {
        SDL_DestroyTexture(enemy.texture);
    }
}

void SceneMain::keyboardControl(float deltaTime)
{
    auto keyboardState = SDL_GetKeyboardState(NULL);
    if(keyboardState[SDL_SCANCODE_W])
    {
        std::cout << "W" << std::endl;
        player.position.y -= player.speed * deltaTime;
    }
    if(keyboardState[SDL_SCANCODE_S])
    {
        std::cout << "S" << std::endl;
        player.position.y += player.speed * deltaTime;
    }
    if(keyboardState[SDL_SCANCODE_A])
    {
        std::cout << "A" << std::endl;
        player.position.x -= player.speed * deltaTime;
    }
    if(keyboardState[SDL_SCANCODE_D])
    {
        std::cout << "D" << std::endl;
        player.position.x += player.speed * deltaTime;
    }

    // 限制玩家在窗口内移动
    if(player.position.x < 0)
    {
        player.position.x = 0;
    }
    if(player.position.x > game.getWindowWidth() - player.width)
    {
        player.position.x = game.getWindowWidth() - player.width;
    }
    if(player.position.y < 0)
    {
        player.position.y = 0;
    }
    if(player.position.y > game.getWindowHeight() - player.height)
    {
        player.position.y = game.getWindowHeight() - player.height;
    }

    // 用空格表示发射子弹
    if(keyboardState[SDL_SCANCODE_J])
    {
        std::cout << "space" << std::endl;
        auto currentTime = SDL_GetTicks();
        if(currentTime - player.lastFireTime >= player.coolDown)
        {
            shootBullet();
            player.lastFireTime = currentTime;
        }
    }

}

void SceneMain::shootBullet()
{
    Bullet* newBullet = new Bullet(bullet);
    newBullet->position.x = player.position.x + player.width / 2 - newBullet->width / 2;
    newBullet->position.y = player.position.y;
    bullets.push_back(newBullet);
}

void SceneMain::updateBullet(float deltaTime)
{
    int margin = 32;
    for(auto it = bullets.begin(); it != bullets.end(); )
    {
        auto cur_bullet = *it;

        // 更新子弹的位置
        cur_bullet->position.y -= cur_bullet->speed * deltaTime;

        if(cur_bullet->position.y + margin < 0)
        {
            it = bullets.erase(it);
            delete cur_bullet;
        }
        else 
        {
            ++it;
        }
    }
}


void SceneMain::renderBullet()
{
    for(auto it : bullets)
    {
        SDL_Rect bulletRect = {static_cast<int>(it->position.x), static_cast<int>(it->position.y), it->width, it->height};
        SDL_RenderCopy(game.getRenderer(), it->texture, nullptr, &bulletRect);
    }
}

void SceneMain::spawnEnemy()
{
    // 生成敌机
    double probability = 1/60.0;
    double random = dis(gen);
    if(random > probability)
    {
        std::cout << "no spawn" << std::endl;
        return;
    }

    // 创建敌机
    Enemy* newEnemy = new Enemy(enemy);
    newEnemy->position.x = dis(gen) * (game.getWindowWidth() - newEnemy->width);
    newEnemy->position.y = -newEnemy->height;
    enemies.push_back(newEnemy);
}

void SceneMain::updateEnemy(float deltaTime)
{
    for(auto it = enemies.begin(); it != enemies.end(); )
    {
        auto cur_enemy = *it;

        // 更新敌机的位置
        cur_enemy->position.y += cur_enemy->speed * deltaTime;

        // 检出敌机位置是否飞出屏幕底部
        if(cur_enemy->position.y > game.getWindowHeight())
        {
            delete cur_enemy;
            it = enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void SceneMain::renderEnemy()
{
    for(auto it : enemies)
    {
        SDL_Rect enemyRect = {static_cast<int>(it->position.x), static_cast<int>(it->position.y), it->width, it->height};
        SDL_RenderCopy(game.getRenderer(), it->texture, nullptr, &enemyRect);
    }
}
