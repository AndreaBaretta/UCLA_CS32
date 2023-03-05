#ifndef ACTOR_H_
#define ACTOR_H_

#include <iostream>
#include <random>
#include <tuple>
#include <utility>
#include "GameConstants.h"
#include "GraphObject.h"
#include "StudentWorld.h"

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
  constexpr const static std::pair<int, int> MOVES[] = {{2, 0},
                                                        {-2, 0},
                                                        {0, 2},
                                                        {0, -2}};

 private:
  bool m_alive;

 protected:
  template <typename... Args>
  Actor(Args&&... args)
      : GraphObject(std::forward<Args>(args)...), m_alive(true) {}

 public:
  virtual void update(StudentWorld* world) = 0;
  virtual void die(StudentWorld* world);
  virtual bool isImpactible() const;
  virtual void impact();
  bool isAlive() const;
};

class Avatar : public Actor {
 public:
  enum State { WAITING_TO_ROLL, WALKING };

 private:
  int m_ticks_to_move;
  State m_state;
  WalkDirection m_walk_direction;
  int m_coins;
  int m_stars;
  bool m_has_vortex;

 protected:
  Avatar(int imageID, int x, int y);

 public:
  virtual int getAction(StudentWorld* world) = 0;
  virtual void update(StudentWorld* world);
  int remainingSteps();
  int getCoins();
  int getStars();
  bool hasVortex();
  void addCoins(int coins);
  void addStars(int stars);
  void giveVortex();
  bool isWalking() const;
  void setWalkDirection(WalkDirection direction);
  void teleport(int new_x, int new_y);
  void swap(Avatar* other);
};

class Peach : public Avatar {
 public:
  Peach(int x, int y);
  virtual int getAction(StudentWorld* world);
};

class Yoshi : public Avatar {
 public:
  Yoshi(int x, int y);
  virtual int getAction(StudentWorld* world);
};

class Square : public Actor {
 private:
  bool m_hasActivatedOnPeach;
  bool m_hasActivatedOnYoshi;

 protected:
  Square(int imageID, int x, int y);

 public:
  bool isOn(Avatar* avatar);
  // Calling effect does not consider whether or not the square's effects are
  // for transient avatars or one that finish their roll there
  virtual void update(StudentWorld* world);
  virtual void effect(StudentWorld* world, Avatar* avatar) = 0;
};

class BlueCoinSquare : public Square {
 public:
  BlueCoinSquare(int x, int y);

  virtual void effect(StudentWorld* world, Avatar* avatar);

  virtual void die(StudentWorld* world);
};

class RedCoinSquare : public Square {
 public:
  RedCoinSquare(int x , int y);

  virtual void effect(StudentWorld* world, Avatar* avatar);
};

class StarSquare : public Square {
 public:
  StarSquare(int x, int y);

  virtual void effect(StudentWorld* world, Avatar* avatar);
};

class DirectionalSquare : public Square {
 private:
  WalkDirection m_force;

 public:
  DirectionalSquare(int imageID, int x, int y, int direction);

  virtual void effect(StudentWorld* world, Avatar* avatar);
};

class BankSquare : public Square {
 public:
  BankSquare(int x, int y);

  virtual void effect(StudentWorld* world, Avatar* avatar);
};

class EventSquare : public Square {
 public:
  EventSquare(int x, int y);

  virtual void effect(StudentWorld* world, Avatar* avatar);
};

class Vortex : public Actor {
 private:
  const WalkDirection m_walk_direction;

 public:
  Vortex(WalkDirection direction, int x, int y)
      : Actor(IID_VORTEX,
              x, y,
              GraphObject::right,
              0,
              1.0),
        m_walk_direction(direction) {}

  virtual void update(StudentWorld* world);
};

#endif  // ACTOR_H_
