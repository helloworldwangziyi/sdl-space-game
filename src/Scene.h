//
// Created by Administrator on 25-5-11.
//

#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>

class Game;
class Scene
{
public:
    Scene();
    virtual ~Scene() = default;

    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual void clean() = 0;
    virtual void handleEvent(SDL_Event* event) = 0;

protected:
    Game& game;
};


#endif //SCENE_H
