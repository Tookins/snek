//snake game
//TODO: 
//Add a button to the title screen that starts the game
//Create game objects and logic

#include <iostream>
#include <SDL2/SDL.h>
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

//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

//helper functions
bool initGame();

bool loadTitle();

void shutdown();

SDL_Surface* loadSurface(std::string path);

//global variables
SDL_Window* gameWindow = NULL;

SDL_Surface* gameSurface = NULL; 

SDL_Surface* currSurface = NULL;

SDL_Surface* keyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

//game entry point
int main(int argc, char *argv[]){
    
    if(!initGame()){
    
        printf("failed to load game");
        
    } else {		

        //load the title screen

        if ( !loadTitle()) {

            printf("failed to load title screen");

        } else {
        
            bool quit = false;

            SDL_Event e;
            
            GameStates state = TITLE_SCREEN;

            //main loop 
            while (!quit){
                    
                while(SDL_PollEvent(&e) != 0){
                    
                    if(e.type == SDL_QUIT){
                    
                        quit = true;
                    
                    } else if (state == TITLE_SCREEN && e.type == SDL_MOUSEBUTTONUP) {
                    
                        //set the screen surface to the game
                        state = GAME_SCREEN;

                        currSurface = loadSurface("game_screen.bmp");                        

                    } else if (state == GAME_SCREEN && e.type == SDL_MOUSEBUTTONUP) {

                        state = SCORE_SCREEN;

                        currSurface = loadSurface("score_screen.bmp");

                    }

                }
            
                SDL_BlitSurface(currSurface, NULL, gameSurface, NULL);
            
                SDL_UpdateWindowSurface(gameWindow);
            
            }
        }    
    }

    SDL_Delay(500); //adds a half second pause after quitting before window closes
	
	SDL_DestroyWindow(gameWindow);
	
	SDL_Quit();
	
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

           gameSurface = SDL_GetWindowSurface(gameWindow); 
    
        }

    }
    return success;

}


//loads a game resource from a file 
bool loadTitle(){

    bool success = true;

    currSurface = SDL_LoadBMP("title_screen.bmp");
        
    if(currSurface == NULL ){
         
        printf( "Unable to load image %s! SDL Error: %s\n", "title_screen.bmp", SDL_GetError() );
            
        success = false;
    }
        
    return success;

}

//loads an image at the specified path
SDL_Surface* loadSurface(std::string path) {

    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());

    if ( loadedSurface == NULL ) {

        printf("unable to load image %s!, SDL error: %s\n", path.c_str(), SDL_GetError());

    }

    return loadedSurface;

}
