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

//screen dimension constants
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;

//game states
enum GameStates
{
    TITLE_SCREEN,
    GAME_SCREEN,
    SCORE_SCREEN
};

//helper function prototypes
bool initGame();

bool loadTitle();

void shutdown();


SDL_Texture* loadTexture(std::string path);

//global variables
SDL_Window* gameWindow = NULL;

SDL_Surface* gameSurface = NULL; 

SDL_Renderer* gameRenderer = NULL;

SDL_Texture* gameTexture = NULL;

SDL_Rect gameRect;

GameStates gameState;

//snake entity class
class Snake {
private:
    int x_pos;
    int y_pos;
    int x_vel;
    int y_vel;
    SDL_Texture* texture;
    SDL_Rect rect;

public:
    Snake(SDL_Texture* textorsmt) {
        x_pos = 0;
        y_pos = 0;
        x_vel = 0;
        y_vel = 0;
        texture = textorsmt;
        rect = {x_pos, y_pos, 32, 32};
    }
    int get_xpos(){
        return x_pos;
    }
    int get_ypos(){
        return y_pos;
    }
    int get_xvel(){
        return x_vel;
    }
    int get_yvel(){
        return y_vel;
    }
    void set_pos(int x, int y){
        x_pos = x;
        y_pos = y;
    }
    void set_vel(int v_x, int v_y){
        x_vel = v_x;
        y_vel = v_y;
    }
    SDL_Texture* get_texture(){
        return texture;
    }
    SDL_Rect get_rect(){
        return rect;
    }
    void move(){
        x_pos = x_pos + x_vel;
        y_pos = y_pos + y_vel;
        rect.x = x_pos;
        rect.y = y_pos;
    }
};


//game entry point
int main(int argc, char *argv[]){
    
    if(!initGame()){
    
        printf("failed to load game");
        
    } else {		

        bool quit = false;

        SDL_Event e;

        //instantiate snake object
        Snake* snek = new Snake(loadTexture("images/snekkyBoy.png"));

        //main loop 
        while (!quit){

            while(SDL_PollEvent(&e) != 0){
                
                if(e.type == SDL_QUIT){
                
                    quit = true;
                
                } else if (gameState == TITLE_SCREEN) {

                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                    
                        //set the screen surface to the game
                        gameState = GAME_SCREEN;
                        
                    }
                                    
                    SDL_RenderClear(gameRenderer);

                    SDL_RenderCopy(gameRenderer, gameTexture, NULL, NULL);
                
                    SDL_RenderPresent(gameRenderer);

                } else if (gameState == GAME_SCREEN) {
                    
                    //TODO: render the background here


                    //listens for keypresses to move the snake around the screen
                    if (e.type == SDL_KEYDOWN) {

                        switch (e.type) {

                            case SDLK_UP:
                                snek->set_vel(0, 1);
                                break;
                
                            case SDLK_DOWN:
                                snek->set_vel(0, -1);
                                break;
                            
                            case SDLK_LEFT:
                                snek->set_vel(-1, 0);
                                break;
                            
                            case SDLK_RIGHT:
                                snek->set_vel(1, 0);
                            
                            case SDLK_ESCAPE:
                                gameState = SCORE_SCREEN;
                                break;

                            default:
                            break;
                        }
                    }

                    snek->move();

                    gameRect = snek->get_rect();

                    gameTexture = snek->get_texture();

                    SDL_RenderClear(gameRenderer);

                    SDL_RenderCopy(gameRenderer, gameTexture, NULL, &gameRect);
                    
                    SDL_RenderPresent(gameRenderer);

                } else if (gameState == SCORE_SCREEN) {

                    if (e.type == SDL_MOUSEBUTTONDOWN) {

                        gameState = TITLE_SCREEN;

                    }
                                    
                    SDL_RenderClear(gameRenderer);

                    SDL_RenderCopy(gameRenderer, gameTexture, NULL, NULL);
                
                    SDL_RenderPresent(gameRenderer);
                }
            }
        }    
        delete snek;
   }
    shutdown();
	return 0;
}

//returns true if the game window is successfully initialized

bool initGame() {
    
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        
        success = false;

        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        
    }else{

        gameWindow = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        
        if(gameWindow == NULL){
            
            success = false;
            
            printf("the window is broken: %s/n", SDL_GetError());
        
        }else{
            
            gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gameRenderer == NULL) {

                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            } else {

                SDL_SetRenderDrawColor(gameRenderer, 0xff, 0xff, 0xff, 0xff);

                int imgFlags = IMG_INIT_PNG;

                if (!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG)) {
                    
                    printf("SDL_Image could not initialize! SDL_Image Error: %s\n", IMG_GetError());
                 
                    success = false;
                
                } else {

                    gameState = TITLE_SCREEN; 
                
                    gameSurface = SDL_GetWindowSurface(gameWindow);

                    gameTexture = loadTexture("images/title_screen.png");
        
                }
            }
        }
    }

    return success;
}

//loads a texture located at the specified path
SDL_Texture* loadTexture(std::string path) {

    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == NULL) {

        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());

    } else {

        newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);

        if (newTexture == NULL) {

            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());

        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

//closes SDL and frees up allocated memory
void shutdown() {

    //destroy loaded texture
    SDL_DestroyTexture(gameTexture);
    gameTexture = NULL;

    //destroy game window
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(gameWindow);
    gameRenderer = NULL;
    gameWindow = NULL;
    
    //quit sdl
    IMG_Quit();
    SDL_Quit();
}
