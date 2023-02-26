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
 private:
  bool m_alive;

 protected:
  template <typename... Args>
  Actor(Args&&... args)
      : GraphObject(std::forward<Args>(args)...), m_alive(true) {}

 public:
  virtual void update(StudentWorld* world) = 0;
  virtual void die(StudentWorld* world);
  bool isAlive();
};

class Avatar : public Actor {
 public:
  constexpr const static std::pair<int, int> MOVES[] = {{2, 0},
                                                        {-2, 0},
                                                        {0, 2},
                                                        {0, -2}};
  enum State { WAITING_TO_ROLL, WALKING };

 private:
  int m_ticks_to_move;
  State m_state;
  WalkDirection m_walk_direction;
  int m_coins;
  int m_stars;
  bool m_has_vortex;

 protected:
  template <typename... Args>
  Avatar(int imageID, Args&&... args)
      : Actor(imageID, std::forward<Args>(args)..., GraphObject::right, 0, 1.0),
        m_ticks_to_move(0),
        m_state(WAITING_TO_ROLL),
        m_walk_direction(WalkDirection::RIGHT),
        m_coins(0),
        m_stars(0),
        m_has_vortex(false) {}

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
  const bool isWalking() const;
};

class Peach : public Avatar {
 public:
  template <typename... Args>
  Peach(Args&&... args) : Avatar(IID_PEACH, std::forward<Args>(args)...) {}
  virtual int getAction(StudentWorld* world);
};

class Yoshi : public Avatar {
 public:
  template <typename... Args>
  Yoshi(Args&&... args) : Avatar(IID_YOSHI, std::forward<Args>(args)...) {}
  virtual int getAction(StudentWorld* world);
};

class Square : public Actor {
 protected:
  bool m_hasActivatedOnPeach;
  bool m_hasActivatedOnYoshi;

  template <typename... Args>
  Square(int imageID, Args&&... args)
      : Actor(imageID, std::forward<Args>(args)...),
        m_hasActivatedOnPeach(false),
        m_hasActivatedOnYoshi(false) {}

 public:
  bool isOn(Avatar* avatar);
  // Calling effect does not consider whether or not the square's effects are
  // for transient avatars or one that finish their roll there
  virtual void update(StudentWorld* world);
  virtual void effect(StudentWorld* world, Avatar* avatar) = 0;
};

class BlueCoinSquare : public Square {
 public:
  template <typename... Args>
  BlueCoinSquare(Args&&... args)
      : Square(IID_BLUE_COIN_SQUARE,
               std::forward<Args>(args)...,
               GraphObject::right,
               1,
               1.0) {}

  virtual void effect(StudentWorld* world, Avatar* avatar);

  virtual void die(StudentWorld* world);
};

class RedCoinSquare : public Square {
 public:
  template <typename... Args>
  RedCoinSquare(Args&&... args)
      : Square(IID_RED_COIN_SQUARE,
               std::forward<Args>(args)...,
               GraphObject::right,
               1,
               1.0) {}

  void effect(StudentWorld* world, Avatar* avatar);
};

#endif  // ACTOR_H_
