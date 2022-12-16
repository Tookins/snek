#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>
#include <list>
#include <tuple>

//snake entity class
class Snake {
private:
    int m_vX;
    int m_vY;
    int m_length;
    SDL_Rect m_head;
    std::list<std::pair<int,int>> m_body;

public:
    Snake(int len=1, int X=0, int Y=0, int vX=0, int vY=0);
    int getVX();
    int getVY();
    int getLength();
    SDL_Rect getHead();
    std::list<std::pair<int,int>> getBody();
    void setLength(int len = 0);
    void setPos(int X=0, int Y=0);
    void setVel(int vX=0, int vY=0);
    void update(bool onTarget = false);
};

#endif