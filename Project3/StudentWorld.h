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
    int m_bank;

   public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    int getBank() const;
    void clearBank();
    int addCoins(int coins);
    bool canMoveInDirection(int x, int y, int direction) const;
    bool canMoveRight(int x, int y) const;
    bool canMoveLeft(int x, int y) const;
    bool canMoveUp(int x, int y) const;
    bool canMoveDown(int x, int y) const;
    Peach* getPeach();
    Yoshi* getYoshi();
};

#endif  // STUDENTWORLD_H_
