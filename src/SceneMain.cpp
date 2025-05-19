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
}

void SceneMain::Render()
{
    SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
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
}

void SceneMain::clean()
{
    if (player.texture != nullptr)
    {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
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

}
