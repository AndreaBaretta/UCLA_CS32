#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and
// StudentWorld.cpp

// const int IID_PEACH = 0;
// const int IID_YOSHI = 1;
// const int IID_BLUE_COIN_SQUARE = 2;
// const int IID_RED_COIN_SQUARE = 3;
// const int IID_DIR_SQUARE = 4;
// const int IID_EVENT_SQUARE = 5;
// const int IID_BANK_SQUARE = 6;
// const int IID_STAR_SQUARE = 7;
// const int IID_DROPPING_SQUARE = 8;
// const int IID_BOWSER = 9;
// const int IID_BOO = 10;
// const int IID_VORTEX = 11;

class Actor : public GraphObject {
  public:
    template<typename... Args>
    Actor(Args&&... args) : GraphObject(std::forward<Args>(args)...) {}

    virtual void update(StudentWorld* world) = 0;
};

class Player : public Actor {
  public:
    template<typename... Args>
    Player(Args&&... args) : Actor(std::forward<Args>(args)...) {}

    virtual void update(StudentWorld* world) {
      switch (m_imageID) {
        case ()
      }
    }
};

#endif  // ACTOR_H_
