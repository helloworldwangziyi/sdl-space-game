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
}

void SceneMain::Render()
{

    // 渲染玩家
    SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);

    // 渲染子弹
    renderBullet();

    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &playerRect);
}

void SceneMain::handleEvent(SDL_Event *event)
{
}

void SceneMain::Init()
{
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
    if(keyboardState[SDL_SCANCODE_SPACE])
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
