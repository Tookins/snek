//snake game
//TODO: 
//Add a button to the title screen that starts the game
//Create game objects and logic
//Add text output to the game over screen
//add a way to play again after game over
//add in music

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "GameRenderer.hpp"
#include "Snake.hpp"

#define fps 60
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

//game entry point
int main(int argc, char *argv[]){

    //initialize SDL and SDL_image
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        printf("SDL failed to init! SDL Error: %s\n", SDL_GetError());
    }
    if (!(IMG_Init(IMG_INIT_PNG))) {
        printf("IMG failed to init! IMG Error: %s\n", IMG_GetError());
    }

    //initialize render window    
    GameRenderer gWindow("GAME v0.1", SCREEN_WIDTH, SCREEN_HEIGHT);

    //load title screen

    
    //quit flag
    bool quit = false;

    //instantiate snake object
    Snake* snek = new Snake(gWindow.loadTexture("images/snekkyBoy.png"));

    // set game texture
    SDL_Texture* gTexture = snek->get_texture();

    //limit frame rate
    Uint32 startingTick;

    //main loop 
    while (!quit)
    {
        //event for game inputs
        SDL_Event gEvent;

        //get time from initial tick
        startingTick = SDL_GetTicks();

        while(SDL_PollEvent(&gEvent) != 0)
        {    

            if(gEvent.type == SDL_QUIT) 
            {
                quit = true;
            }
            //listens for keypresses to move the snake around the screen
            else if (gEvent.type == SDL_KEYDOWN) 
            {
                switch (gEvent.key.keysym.sym) 
                {
                    case SDLK_UP:
                        snek->set_vel(0, -8);
                        break;
        
                    case SDLK_DOWN:
                        snek->set_vel(0, 8);
                        break;
                    
                    case SDLK_LEFT:
                        snek->set_vel(-8, 0);
                        break;
                    
                    case SDLK_RIGHT:
                        snek->set_vel(8, 0);
                        break;
                    
                    case SDLK_ESCAPE:
                        quit = true;
                        break;

                    default:
                    break;
                }
            }
        }

        snek->move();

        SDL_Rect dstRect = snek->get_rect();

        gTexture = snek->get_texture();
    
        gWindow.clear();

        gWindow.render(gTexture, NULL, &dstRect);

        gWindow.display();

        if (1000/fps > SDL_GetTicks() - startingTick)
        {
            SDL_Delay(1000/fps - (SDL_GetTicks() - startingTick));
        }
    }
    
    //free up game entity memory    
    delete snek;
    //destroy game window
    gWindow.destroy();
    
    //destroy loaded texture
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    //quit sdl
    IMG_Quit();
    SDL_Quit();

    return 0;
}