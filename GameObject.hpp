// an object that will keep data relevant to the particular instance of
// the game. at least needs user name, score, and timestamps

// include statements
#include <tuple>
#include <list>
#include <string>
#include <ctime>

// class description
class GameObject
{
public:
    GameObject();
    double getTimeElapsed();
    int getScore();
    void setScore(std::list<std::pair<int, int>> obj);

private:
    int m_score;
    time_t *m_initT;
};