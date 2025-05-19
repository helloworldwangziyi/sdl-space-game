//
// Created by Administrator on 25-5-11.
//
#include "Game.h"
#include "SceneMain.h"
#include <SDL.h>

Game::Game()
{

}

Game::~Game()
{
    clean();
}

void Game::run()
{
    while (isRunning)
    {

        auto frameStart = SDL_GetTicks();
        SDL_Event event;
        handleEvent(&event);

        update(deltaTime);
        render();

        auto frameEnd = SDL_GetTicks();
        auto diff = frameEnd - frameStart;

        if(diff < frameTime)
        {
            SDL_Delay(frameTime - diff);
            deltaTime = frameTime / 1000.0f; // 转换为秒
        }
        else
        {
            deltaTime = diff / 1000.0f;
        }

    }
}

void Game::clean()
{
    if(currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::init()
{
    frameTime = 1000 / FPS;
    // SDL 初始化
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }

    // 创建窗口
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        isRunning = false;
    }
    currentScene = new SceneMain();
    currentScene->Init();
}

void Game::changeScene(Scene* scene)
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->Init();
}

void Game::handleEvent(SDL_Event *event)
{
    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            isRunning = false;
        }
        currentScene->handleEvent(event);
    }
}

void Game::update(float deltaTime)
{
    currentScene->Update(deltaTime);
}

void Game::render()
{

    // 清空渲染器
    SDL_RenderClear(renderer);

    // 渲染场景
    currentScene->Render();

    // 显示渲染器
    SDL_RenderPresent(renderer);
}

