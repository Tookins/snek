#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>

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
    Snake(SDL_Texture* text, int x=0, int y=0, int x_v=0, int y_v=0);
    int get_xpos();
    int get_ypos();
    int get_xvel();
    int get_yvel();
    void set_pos(int x, int y);
    void set_vel(int v_x, int v_y);
    SDL_Texture* get_texture();
    SDL_Rect get_rect();
    void move();
};

#endif