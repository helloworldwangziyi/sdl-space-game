// SceneEnd.cpp
#include "SceneEnd.h"
#include "Game.h"


void SceneEnd::Init()
{
    if (!SDL_IsTextInputActive()){
        SDL_StartTextInput();
    }
    if (!SDL_IsTextInputActive()){
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "SDL", "Failed to start text input: %s", SDL_GetError());
    }
}

void SceneEnd::Update(float deltaTime)
{
    
}

void SceneEnd::Render()
{
    if (isTyping){
        renderPhase1(); 
    }else{
        renderPhase2();
    }
}

void SceneEnd::clean()
{
}

void SceneEnd::handleEvent(SDL_Event *event)
{
    if (isTyping){
        if (event->type == SDL_TEXTINPUT){
            name += event->text.text;
        }
        if (event->type == SDL_KEYDOWN){
            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN){
                isTyping = false;
                SDL_StopTextInput();
            }
            if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE){
                name.pop_back();
            }
        }
    }
    else{
        // TODO: 处理排行榜阶段的事件（后续课程实现）
    }
}

void SceneEnd::renderPhase1()
{
    auto score = game.getFinalScore();
    std::string scoreText = "你的得分是：" + std::to_string(score);
    std::string gameOver = "Game Over";
    std::string instrutionText = "请输入你的名字，按回车键确认：";
    game.renderTextCentered(scoreText, 0.1, false);
    game.renderTextCentered(gameOver, 0.4, true);
    game.renderTextCentered(instrutionText, 0.6, false);

    if (name != ""){
        game.renderTextCentered(name, 0.8, false);
    }
}

void SceneEnd::renderPhase2()
{
    // TODO: 实现第二阶段渲染}
}