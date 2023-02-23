#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <string>
#include <vector>
#include "Board.h"
#include "GameWorld.h"
#include "GraphObject.h"
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
   private:
    Board m_board;
    Peach* m_peach;
    Yoshi* m_yoshi;
    std::vector<Actor*> m_actors;

   public:
    StudentWorld(std::string assetPath);
    virtual int init() {}
    virtual int move() {}
    virtual void cleanUp() {}
    bool canMoveInDirection(int x, int y, int direction) {
        switch (direction) {
            case GraphObject::right:
                return canMoveRight(x, y);
                break;
            case GraphObject::left:
                return canMoveLeft(x, y);
                break;
            case GraphObject::up:
                return canMoveUp(x, y);
                break;
            case GraphObject::down:
                return canMoveDown(x, y);
                break;
            default:
                return false;
        }
    }
    bool canMoveRight(int x, int y);
    bool canMoveLeft(int x, int y);
    bool canMoveUp(int x, int y);
    bool canMoveDown(int x, int y);
    Peach* getPeach();
    Yoshi* getYoshi();
};

#endif  // STUDENTWORLD_H_
