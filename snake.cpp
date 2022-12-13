#include <SDL2/SDL.h>

#include "Snake.hpp"

//screen dimension constants
const Uint32 SCREEN_WIDTH = 1366;
const Uint32 SCREEN_HEIGHT = 768;

//snake entity class
Snake::Snake(SDL_Texture* text, int x, int y, int x_v, int y_v)
{
    x_pos = x%SCREEN_WIDTH;
    y_pos = y%SCREEN_HEIGHT;
    x_vel = x_v;
    y_vel = y_v;
    texture = text;
    rect = {x_pos, y_pos, 32, 32};
}
int Snake::get_xpos(){
    return x_pos;
}
int Snake::get_ypos(){
    return y_pos;
}
int Snake::get_xvel(){
    return x_vel;
}
int Snake::get_yvel(){
    return y_vel;
}
void Snake::set_pos(int x, int y){
    x_pos = x%SCREEN_WIDTH;
    y_pos = y%SCREEN_HEIGHT;
}
void Snake::set_vel(int v_x, int v_y){
    x_vel = v_x;
    y_vel = v_y;
}
SDL_Texture* Snake::get_texture(){
    return texture;
}
SDL_Rect Snake::get_rect(){
    return rect;
}
void Snake::move(){
    x_pos = (x_pos + x_vel)%SCREEN_WIDTH;
    y_pos = (y_pos + y_vel)%SCREEN_HEIGHT;
    rect.x = x_pos;
    rect.y = y_pos;
};

