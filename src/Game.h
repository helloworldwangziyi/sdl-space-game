//
// Created by Administrator on 25-5-11.
//

#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include <SDL_mixer.h>
#include "Object.h"
#include "SceneTitle.h"
#include <string>

class Game
{
public:
    static Game& getInstance()
    {
        static Game instance;
        return instance;
    }

    ~Game();
    void run();
    void init();
    void clean();
    void changeScene(Scene* scene);

    void handleEvent(SDL_Event *event);
    void update(float deltaTime);
    void render();

    SDL_Window* getWindow() { return window; }
    SDL_Renderer* getRenderer() {return renderer;}
    int getWindowWidth() {return windowWidth;}
    int getWindowHeight() {return windowHeight;}

    void updateBackground(float deltaTime); // 更新背景
    void renderBackground(); // 渲染背景


    // 渲染文字函数
    void renderTextCentered(std::string text, float posY, bool isTitle);
private:
    Game();
    // 删除拷贝构造函数
    Game(const Game&) = delete;
    // 删除赋值运算符
    Game& operator=(const Game&) = delete;
    Scene* currentScene = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int windowWidth = 600;
    int windowHeight = 800;
    bool isRunning = true;
    int FPS = 60; // 帧率
    Uint32 frameTime; // 帧时间
    float deltaTime; // 时间间隔

    // 背景
    Backgroud nearStars; // 近处的星星
    Backgroud farStars; // 远处的星星

    TTF_Font* titleFont;
    TTF_Font* textFont;
};

#endif //GAME_H
