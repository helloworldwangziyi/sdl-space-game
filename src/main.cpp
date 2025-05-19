//
// Created by Administrator on 25-5-12.
//
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<SDL_image.h>
#include<iostream>
#include "Game.h"
#include "SceneMain.h"
int main(int , char **)
{
    Game &game = Game::getInstance();
    game.init();
    game.run();
    return 0;
}


