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
#define fps 6
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

    // instantiate game objects
    Snake *snek = new Snake(1, 0, 0, 0, TEXTURE_HEIGHT);

    //load in game textures
    SDL_Texture *headTexture = gWindow.loadTexture("images/snekkyBoy.png");
    SDL_Texture *bodyTexture = gWindow.loadTexture("images/snekBody.png");
    SDL_Texture *foodTexture = gWindow.loadTexture("images/snekFood.png");

    //render the head of the snake in its initial position
    SDL_Rect headRect = snek->getHead();
    gWindow.render(headTexture, NULL, &headRect);
    
    //render the body of the snake if necessary
    SDL_Rect bodyRect = {0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT};
    for (std::pair<int, int> segment : snek->getBody())
    {
        bodyRect.x = std::get<0>(segment);
        bodyRect.x = std::get<0>(segment);
        gWindow.render(bodyTexture, NULL, &bodyRect);
    }
    
    //render the snake food in its initial position
    SDL_Rect target = {TEXTURE_WIDTH * (rand() % (SCREEN_WIDTH / TEXTURE_WIDTH))- TEXTURE_WIDTH, 
                      TEXTURE_HEIGHT * (rand() % (SCREEN_HEIGHT / TEXTURE_HEIGHT)) - TEXTURE_HEIGHT, 
                      TEXTURE_WIDTH, TEXTURE_HEIGHT};
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

        //flag for checking if snake hits the target (food)
        bool onTarget = false;

        //reads through the event queue and moves according to arrow keys
        //or closes on esc key
        while (SDL_PollEvent(&gameEvent) != 0)
        {
            //closes the window if user clicks the 'x' button
            if (gameEvent.type == SDL_QUIT)
            {
                quit = true;
            }
            //when a key is pressed, changes direction or closes the program
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

        //clear the window and render the textures for the next frame
        gWindow.clear();

        //if the snakes head is on the same rect as the target
        //set the update flag to true and change the position
        //of the target if necessary
        if (target.x == headRect.x && target.y == headRect.y)
        {
            onTarget = true;
            target.x = TEXTURE_WIDTH * (rand() % (SCREEN_WIDTH / TEXTURE_WIDTH) + 1)- TEXTURE_WIDTH;
            target.y = TEXTURE_HEIGHT * (rand() % (SCREEN_HEIGHT / TEXTURE_HEIGHT) + 1) - TEXTURE_HEIGHT;
        }

        //update the position of the snake and increase length by 1  
        //if onTarget == true
        snek->update(onTarget);
 
        //add the target texture to the renderer
        gWindow.render(foodTexture, NULL, &target);
       
        //add the head of the snake to the renderer
        headRect = snek->getHead();
        gWindow.render(headTexture, NULL, &headRect);
        
        //add the body of the snake to the renderer
        for (std::pair<int, int> segment : snek->getBody())
        {
            bodyRect.x = std::get<0>(segment);
            bodyRect.y = std::get<1>(segment);
            gWindow.render(bodyTexture, NULL, &bodyRect);
        }

        //display the renderer to the screen
        gWindow.display();

        // delay next frame to hold fps constant (defined at the top of main)
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
    SDL_DestroyTexture(foodTexture);

    // quit sdl
    IMG_Quit();
    SDL_Quit();

    return 0;
}