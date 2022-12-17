//an object that will keep data relevant to the particular instance of 
//the game. at least needs user name, score, and timestamps

//include statements
#include <ctime>
#include "GameObject.hpp"

//constructor
GameObject::GameObject(std::string name)
{
    m_name = name;
    time_t* init;
    time(init);
    m_initT = init;
    m_score = 0;
}
std::string GameObject::getName()
{
    return m_name;
}
void GameObject::setName(std::string name)
{
    m_name = name;
}
double GameObject::getTimeElapsed()
{
    time_t* currT;
    time(currT);
    return std::difftime(*currT, *m_initT);
}
int GameObject::getScore()
{
    return m_score;
}
void GameObject::setScore(std::list<std::pair<int,int>> obj)
{
    m_score = obj.size();
}