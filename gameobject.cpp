// an object that will keep data relevant to the particular instance of
// the game. at least needs user name, score, and timestamps

// include statements
#include <ctime>
#include "GameObject.hpp"

// constructor
GameObject::GameObject()
{
    time(m_initT);
    m_score = 0;
}
double GameObject::getTimeElapsed()
{
    time_t *currT;
    time(currT);
    return std::difftime(*currT, *m_initT);
}
int GameObject::getScore()
{
    return m_score;
}
void GameObject::setScore(std::list<std::pair<int, int>> obj)
{
    m_score = obj.size();
}