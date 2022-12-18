// implementation of GameRenderer class

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstring>
#include <stdio.h>

#include "GameRenderer.hpp"

GameRenderer::GameRenderer(std::string name, int window_width, int window_height)
    : window{nullptr}, renderer{nullptr}
{
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              window_width, window_height, SDL_WINDOW_SHOWN);

    if (window == nullptr)
    {
        printf("Window failed to initialize! SDL Error: %s\n", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture *GameRenderer::loadTexture(std::string tex_path)
{

    SDL_Texture *gTexture = NULL;

    gTexture = IMG_LoadTexture(renderer, tex_path.c_str());

    if (gTexture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", tex_path.c_str(), SDL_GetError());
    }

    return gTexture;
}

void GameRenderer::destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void GameRenderer::clear()
{
    SDL_RenderClear(renderer);
}

void GameRenderer::renderImage(SDL_Texture *tex, SDL_Rect *src, SDL_Rect *dst)
{
    SDL_RenderCopy(renderer, tex, src, dst);
}

void GameRenderer::renderText(TTF_Font *font, std::string text, SDL_Rect *src, SDL_Rect *dst)
{
    // create an sdl surface
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), {0, 255, 0});
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, textTexture, src, dst);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void GameRenderer::display()
{
    SDL_RenderPresent(renderer);
}