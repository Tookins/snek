// snake game
// TODO:
// Add a button to the title screen that starts the game
// Create game objects and logic
// Add text output to the game over screen
// add a way to play again after game over
// add in music

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <tuple>
#include <cstdlib>
#include "GameRenderer.hpp"
#include "Snake.hpp"

// game constants
#define fps 2
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define TEXTURE_WIDTH 32
#define TEXTURE_HEIGHT 32

// game entry point
int main(int argc, char *argv[])
{

    // initialize SDL and SDL_image
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        printf("SDL failed to init! SDL Error: %s\n", SDL_GetError());
    }
    if (!(IMG_Init(IMG_INIT_PNG)))
    {
        printf("IMG failed to init! IMG Error: %s\n", IMG_GetError());
    }

    // initialize render window
    GameRenderer gWindow("GAME v0.1", SCREEN_WIDTH, SCREEN_HEIGHT);

    // load title screen

    // quit flag
    bool quit = false;

    // instantiate snake object
    Snake *snek = new Snake(5, 0, 0, 0, TEXTURE_HEIGHT);

    // set snake texture and render at initial position
    SDL_Texture *headTexture = gWindow.loadTexture("images/snekkyBoy.png");
    SDL_Texture *bodyTexture = gWindow.loadTexture("images/snekBody.png");
    SDL_Rect dstRect = {0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT};
    SDL_Rect srcRect = snek->getHead();
    gWindow.render(headTexture, NULL, &srcRect);
    for (std::pair<int, int> segment : snek->getBody())
    {
        dstRect.x = std::get<0>(segment);
        dstRect.x = std::get<0>(segment);
        gWindow.render(bodyTexture, NULL, &dstRect);
    }
    
    //render the snake food
    SDL_Rect target = {rand() % SCREEN_WIDTH - TEXTURE_WIDTH, rand() % SCREEN_HEIGHT - TEXTURE_HEIGHT, TEXTURE_WIDTH, TEXTURE_HEIGHT};
    gWindow.render(gWindow.loadTexture("images/snekFood.png"), NULL, &target);
    
    //draw the initial conditions for the game
    gWindow.display();

    // limit frame rate
    Uint32 startingTick;

    // main loop
    while (!quit)
    {
        // event for game inputs
        SDL_Event gameEvent;

        // get time from initial tick
        startingTick = SDL_GetTicks();

        while (SDL_PollEvent(&gameEvent) != 0)
        {

            if (gameEvent.type == SDL_QUIT)
            {
                quit = true;
            }
            // listens for keypresses to move the snake around the screen
            else if (gameEvent.type == SDL_KEYDOWN)
            {
                switch (gameEvent.key.keysym.sym)
                {
                case SDLK_UP:
                    snek->setVel(0, -1 * TEXTURE_HEIGHT);
                    break;

                case SDLK_DOWN:
                    snek->setVel(0, TEXTURE_HEIGHT);
                    break;

                case SDLK_LEFT:
                    snek->setVel(-1 * TEXTURE_WIDTH, 0);
                    break;

                case SDLK_RIGHT:
                    snek->setVel(TEXTURE_WIDTH, 0);
                    break;

                case SDLK_ESCAPE:
                    quit = true;
                    continue;
                    break;

                default:
                    break;
                }
            }
        }

        snek->update();

        dstRect = snek->getHead();

        gWindow.clear();

        gWindow.render(gWindow.loadTexture("images/snekFood.png"), NULL, &target);

        gWindow.render(headTexture, NULL, &dstRect);

        for (std::pair<int, int> segment : snek->getBody())
        {
            dstRect.x = std::get<0>(segment);
            dstRect.y = std::get<1>(segment);
            gWindow.render(bodyTexture, NULL, &dstRect);
        }
        // set the current position of the head to its new location
        srcRect = snek->getHead();

        gWindow.display();

        if (1000 / fps > SDL_GetTicks() - startingTick)
        {
            SDL_Delay(1000 / fps - (SDL_GetTicks() - startingTick));
        }
    }

    // free up game entity memory
    delete snek;
    // destroy game window
    gWindow.destroy();

    // destroy loaded textures
    SDL_DestroyTexture(headTexture);
    SDL_DestroyTexture(bodyTexture);

    // quit sdl
    IMG_Quit();
    SDL_Quit();

    return 0;
}