// class for handling the window the game will be rendered to
#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class GameRenderer
{
public:
    GameRenderer(std::string name, int window_width, int window_height);
    SDL_Texture *loadTexture(std::string tex_path);
    void destroy();
    void clear();
    void renderImage(SDL_Texture *tex, SDL_Rect *src, SDL_Rect *dst);
    void renderText(TTF_Font *font, std::string text, SDL_Rect *src, SDL_Rect *dst);
    void display();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};

#endif