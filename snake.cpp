#include "Snake.hpp"

// screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

// texture dimension constant
const int TEXTURE_WIDTH = 32;
const int TEXTURE_HEIGHT = 32;

// snake entity class
Snake::Snake(int len, int X, int Y, int vX, int vY)
{
    m_length = len;
    m_vX = vX;
    m_vY = vY;
    m_head = {(X + SCREEN_WIDTH) % SCREEN_WIDTH, (Y + SCREEN_HEIGHT) % SCREEN_HEIGHT, TEXTURE_WIDTH, TEXTURE_HEIGHT};
    for (int i = 1; i < len; i++)
    {
        m_body.push_back({(m_head.x + i * TEXTURE_WIDTH) % SCREEN_WIDTH, m_head.y});
    }
}
int Snake::getVX()
{
    return m_vX;
}
int Snake::getVY()
{
    return m_vY;
}
int Snake::getLength()
{
    return m_length;
}
SDL_Rect Snake::getHead()
{
    return m_head;
}
std::list<std::pair<int,int>> Snake::getBody()
{
    return m_body;
}
void Snake::setLength(int len)
{
    m_length = len;
}
void Snake::setPos(int X, int Y)
{
    m_head.x = (X + SCREEN_WIDTH) % SCREEN_WIDTH;
    m_head.y = (Y + SCREEN_HEIGHT) % SCREEN_HEIGHT;
}
void Snake::setVel(int vX, int vY)
{
    m_vX = vX;
    m_vY = vY;
}
void Snake::addSegment()
{
    //add a segment to the end of the snake in the direction the tail is pointing
    m_length++;
    m_body.push_back({(std::get<0>(m_body.back()) + TEXTURE_WIDTH + SCREEN_WIDTH)%SCREEN_WIDTH, 
    (std::get<1>(m_body.back()) +TEXTURE_HEIGHT + SCREEN_HEIGHT)%SCREEN_HEIGHT});
}
void Snake::update()
{
    if (m_length > 1) 
    {
        int x_old = m_head.x;
        int y_old = m_head.y;
        m_body.push_front({x_old, y_old});
        m_body.pop_back();
    }
    m_head.x = (m_head.x + m_vX + SCREEN_WIDTH) % SCREEN_WIDTH;
    m_head.y = (m_head.y + m_vY + SCREEN_HEIGHT) % SCREEN_HEIGHT;    
}
