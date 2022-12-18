// snake game written in c++ using sdl2 and aseprite
// TODO:
// Add a button to the title screen that starts the game
// Create game objects and logic
// Add text output to the game over screen
// add a way to play again after game over
// add in music

#include <SDL2/SDL.h>       //enables sdl windows and rendering
#include <SDL2/SDL_image.h> //enables the loading of png files
#include <SDL2/SDL_mixer.h> //enables the use of audio
#include <SDL2/SDL_ttf.h>   //enables the use of text output
#include <stdio.h>          //used for error messages
#include <string>
#include <tuple>
#include <cstdlib>
#include <algorithm>
#include "GameRenderer.hpp"
#include "GameObject.hpp"
#include "Snake.hpp"

// game constants
#define fps 60
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define TEXTURE_WIDTH 32
#define TEXTURE_HEIGHT 32

// game entry point
int main(int argc, char *argv[])
{

    // initialize SDL, SDL_image, SDL_ttf, and SDL_mixer
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        printf("SDL failed to init! SDL Error: %s\n", SDL_GetError());
    }
    if (!(IMG_Init(IMG_INIT_PNG)))
    {
        printf("IMG failed to init! IMG Error: %s\n", IMG_GetError());
    }
    if (TTF_Init() == -1)
    {
        printf("TTF failed to init! TTF Error: %s\n", TTF_GetError());
    }

    // load in font for game over screen
    TTF_Font *gameOverFont = TTF_OpenFont("./fonts/joystix_monospace.ttf", 32);
    if (gameOverFont == nullptr)
    {
        printf("failed to load font");
    }

    // initialize render window
    GameRenderer gWindow("GAME v0.1", SCREEN_WIDTH, SCREEN_HEIGHT);

    // load in game textures
    SDL_Texture *headTexture = gWindow.loadTexture("images/snekkyBoy.png");
    SDL_Texture *bodyTexture = gWindow.loadTexture("images/snekBody.png");
    SDL_Texture *foodTexture = gWindow.loadTexture("images/snekFood.png");
    SDL_Texture *gameOverTex = gWindow.loadTexture("images/game_over.png");
    SDL_Texture *gameBackground = gWindow.loadTexture("images/game_background.png");
    SDL_Texture *gameTitle = gWindow.loadTexture("images/title_screen.png");

    // game state flags
    bool appRunning = true;
    bool title = true;
    bool gameRunning = false;
    bool gameOver = false;

    // intitialize the game instance
    while (appRunning)
    {
        // start with title screen
        title = true;
        while (title)
        {
            // display the title screen
            gWindow.clear();
            gWindow.renderImage(gameTitle, nullptr, nullptr);
            gWindow.display();

            // check for mouse event to start the game
            SDL_Event initEvent;
            while (SDL_PollEvent(&initEvent) != 0)
            {
                // closes the window if user clicks the 'x' button
                if (initEvent.type == SDL_QUIT)
                {
                    title = false;
                    appRunning = false;
                    break;
                }
                // set the game start flag to true to enter the game loop
                else if (initEvent.type == SDL_KEYDOWN)
                {
                    gameRunning = true;
                    title = false;
                    break;
                }
            }
        }

        // close the game if user quits from the
        // title screen
        if (!appRunning)
        {
            continue;
        }

        // initialize the game instance
        GameObject *instance = new GameObject();
        Snake *snek = new Snake(1, 0, 0, 0, TEXTURE_HEIGHT);
        gWindow.clear();

        // initialize rectangles that will
        // be used to render the snake and
        // render the head of the snake in its initial position
        SDL_Rect headRect = snek->getHead();
        SDL_Rect bodyRect;
        gWindow.renderImage(headTexture, NULL, &headRect);

        // render the snake food in its initial position
        SDL_Rect target = {TEXTURE_WIDTH * (rand() % (SCREEN_WIDTH / TEXTURE_WIDTH)) - TEXTURE_WIDTH,
                           TEXTURE_HEIGHT * (rand() % (SCREEN_HEIGHT / TEXTURE_HEIGHT)) - TEXTURE_HEIGHT,
                           TEXTURE_WIDTH, TEXTURE_HEIGHT};
        gWindow.renderImage(gWindow.loadTexture("images/snekFood.png"), NULL, &target);

        // draw the initial configuration of the game
        gWindow.display();

        // used to get the time each loop begins for controlling
        // framerate
        Uint32 startingTick;

        // main loop
        while (gameRunning)
        {
            // event for game inputs
            SDL_Event gameEvent;

            // get time from initial tick
            startingTick = SDL_GetTicks();

            // flag for checking if snake hits the target (food)
            bool onTarget = false;

            // reads through the event queue and moves according to arrow keys
            // or closes on esc key
            while (SDL_PollEvent(&gameEvent) != 0)
            {
                // closes the window if user clicks the 'x' button
                if (gameEvent.type == SDL_QUIT)
                {
                    gameRunning = false;
                }
                // when a key is pressed, changes direction or closes the program
                else if (gameEvent.type == SDL_KEYDOWN)
                {
                    switch (gameEvent.key.keysym.sym)
                    {
                    case SDLK_UP:
                        if (snek->getVY() == 0)
                        {
                            snek->setVel(0, -1 * TEXTURE_HEIGHT);
                        }
                        break;

                    case SDLK_DOWN:
                        if (snek->getVY() == 0)
                        {
                            snek->setVel(0, TEXTURE_HEIGHT);
                        }
                        break;

                    case SDLK_LEFT:
                        if (snek->getVX() == 0)
                        {
                            snek->setVel(-1 * TEXTURE_WIDTH, 0);
                        }
                        break;

                    case SDLK_RIGHT:
                        if (snek->getVX() == 0)
                        {
                            snek->setVel(TEXTURE_WIDTH, 0);
                        }
                        break;

                    case SDLK_ESCAPE:
                        gameRunning = false;
                        gameOver = true;
                        continue;

                    default:
                        break;
                    }
                }
            }

            // clear the window and render the textures for the next frame
            gWindow.clear();

            // if the snakes head is on the same rect as the target
            // set the update flag to true and change the position
            // of the target if necessary
            if (target.x == headRect.x && target.y == headRect.y)
            {
                onTarget = true;
                target.x = TEXTURE_WIDTH * (rand() % (SCREEN_WIDTH / TEXTURE_WIDTH) + 1) - TEXTURE_WIDTH;
                target.y = TEXTURE_HEIGHT * (rand() % (SCREEN_HEIGHT / TEXTURE_HEIGHT) + 1) - TEXTURE_HEIGHT;
            }

            // update the position of the snake and increase length by 1
            // if onTarget == true
            snek->update(onTarget);
            headRect = snek->getHead();

            // see if the snakes head collides with its body
            // if true, exit game loop to game over loop
            // else, render the next frame
            std::list<std::pair<int, int>> snekCoordinates = snek->getBody();
            std::pair<int, int> headCoordinates = {headRect.x, headRect.y};
            if (std::find(snekCoordinates.begin(), snekCoordinates.end(), headCoordinates) != snekCoordinates.end())
            {
                gameRunning = false;
                gameOver = true;
            }
            else
            {
                // add the target texture to the renderer
                gWindow.renderImage(foodTexture, NULL, &target);

                // add the head of the snake to the renderImageer
                gWindow.renderImage(headTexture, NULL, &headRect);

                // add the body of the snake to the renderer
                for (std::pair<int, int> segment : snek->getBody())
                {
                    bodyRect = {std::get<0>(segment), std::get<1>(segment), TEXTURE_WIDTH, TEXTURE_HEIGHT};
                    gWindow.renderImage(bodyTexture, NULL, &bodyRect);
                }
            }

            // display the renderer to the screen
            gWindow.display();

            // delay next frame to hold fps constant (defined at the top of main)
            if (1000 / fps > SDL_GetTicks() - startingTick)
            {
                SDL_Delay(1000 / fps - (SDL_GetTicks() - startingTick));
            }
        }
        // set the score based on the length of the snake
        instance->setScore(snek->getBody());

        // when the game instance is over, display the users name and score
        // to the game over screen
        std::string username = "Jeff";
        std::string score = std::to_string(instance->getScore());
        SDL_Rect usernameRect = {SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, 32 * (int)username.length(), 32};
        SDL_Rect scoreRect = {SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3 + 36, 32 * (int)score.length(), 32};

        // render the game over screen until the user quits or restarts the game
        while (gameOver)
        {
            gWindow.clear();
            gWindow.renderImage(gameOverTex, nullptr, nullptr);
            gWindow.renderText(gameOverFont, username, nullptr, &usernameRect);
            gWindow.renderText(gameOverFont, score, nullptr, &scoreRect);
            gWindow.display();
            // game over options (quit or play again)
            SDL_Event gameOverEvent;
            while (SDL_PollEvent(&gameOverEvent) != 0)
            {
                if (gameOverEvent.type == SDL_QUIT)
                {
                    gameOver = false;
                    appRunning = false;
                }
                else if (gameOverEvent.type == SDL_KEYDOWN)
                {
                    gameOver = false;
                    appRunning = true;
                }
            }
        }

        // free up game entity memory
        delete snek;
        delete instance;
    }

    // destroy game window
    gWindow.destroy();

    // destroy loaded textures
    SDL_DestroyTexture(headTexture);
    SDL_DestroyTexture(bodyTexture);
    SDL_DestroyTexture(foodTexture);
    SDL_DestroyTexture(gameBackground);
    SDL_DestroyTexture(gameOverTex);

    // quit sdl
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}