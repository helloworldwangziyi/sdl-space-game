//
// Created by Administrator on 25-5-11.
//

#ifndef SCENE_H
#define SCENE_H
#include <SDL.h>

class Scene
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual void clean() = 0;
    virtual void handleEvent(SDL_Event* event) = 0;

};


#endif //SCENE_H
