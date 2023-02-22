#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

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
  protected:
    bool m_alive;
    template<typename... Args>
    Actor(Args&&... args) : GraphObject(std::forward<Args>(args)...), m_alive(true) {}
  public:
    virtual void update(StudentWorld* world) = 0;
    virtual void die(StudentWorld* world) = 0;
    bool isAlive() { return m_alive; }
};

class Player : public Actor {
  protected:
    enum State {
      WAITING_TO_ROLL, WALKING
    };

    int m_ticks_to_move;
    State m_state;

    template<typename... Args>
    Player(int imageID, Args&&... args) : Actor(imageID, std::forward<Args>(args)...), m_ticks_to_move(0), m_state(State::WAITING_TO_ROLL) {
      setDirection(GraphObject::right);
      m_depth = 0;
      m_size = 1;
    }
  public:
    virtual void update(StudentWorld* world) {}
    virtual void die(StudentWorld* world) {}

};



class Peach : public Player {
  public:
    template<typename... Args>
    Peach(Args&&... args) : Player(IID_PEACH, std::forward<Args>(args)...) {}
};

class Yoshi : public Player {
  public:
    template<typename... Args>
    Yoshi(Args&&... args) : Player(IID_YOSHI, std::forward<Args>(args)...) {}
};

#endif  // ACTOR_H_
