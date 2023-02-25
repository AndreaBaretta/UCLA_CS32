#ifndef ACTOR_H_
#define ACTOR_H_

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
  virtual void die(StudentWorld* world) {}
  bool isAlive() { return m_alive; }
};

class Avatar : public Actor {
 public:
  constexpr const static std::pair<int, int> MOVES[] = {{2, 0},
                                                        {-2, 0},
                                                        {0, 2},
                                                        {0, 2}};
  enum State { WAITING_TO_ROLL, WALKING };

  enum class WalkDirection { RIGHT = 0, LEFT = 1, UP = 2, DOWN = 3 };

 private:
  int m_ticks_to_move;
  const int m_player_id;
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
        m_player_id(imageID == IID_PEACH ? 1 : 2),
        m_state(WAITING_TO_ROLL),
        m_walk_direction(WalkDirection::RIGHT),
        m_coins(0),
        m_stars(0),
        m_has_vortex(false) {}

 public:
  virtual void update(StudentWorld* world) {
    switch (m_state) {
      case WAITING_TO_ROLL: {
        int action = world->getAction(m_player_id);
        int die_roll = rand() % 10 + 1;
        switch (action) {
          case ACTION_ROLL:
            m_ticks_to_move = 8 * die_roll;
            m_state = WALKING;
            break;
          default:
            return;
        }

        break;
      }
      case WALKING: {
        if (!world->canMoveInDirection(getX(), getY(), getDirection())) {
          if (m_walk_direction == WalkDirection::RIGHT ||
              m_walk_direction == WalkDirection::LEFT) {
            m_walk_direction = world->canMoveUp(getX(), getY())
                                   ? WalkDirection::UP
                                   : WalkDirection::DOWN;
          } else {
            m_walk_direction = world->canMoveRight(getX(), getY())
                                   ? WalkDirection::RIGHT
                                   : WalkDirection::LEFT;
          }
        }
        if (m_walk_direction == WalkDirection::LEFT) {
          setDirection(GraphObject::left);
        }
        auto [d_x, d_y] = MOVES[(int)m_walk_direction];
        moveTo(getX() + d_x, getY() + d_y);
        --m_ticks_to_move;
        if (m_ticks_to_move == 0) {
          m_state = WAITING_TO_ROLL;
        }
        break;
      }
    }
  }
  int remainingSteps() { return (m_ticks_to_move + 7) % 8; }
  int getCoins() { return m_coins; }
  int getStars() { return m_stars; }
  bool hasVortex() { return m_has_vortex; }
  void addCoins(int coins) { m_coins += coins; }
  void addStars(int stars) { m_stars += stars; }
  void giveVortex() { m_has_vortex = true; }
  const bool isWalking() const { return m_state == WALKING; }
};

class Peach : public Avatar {
 public:
  template <typename... Args>
  Peach(Args&&... args) : Avatar(IID_PEACH, std::forward<Args>(args)...) {}
};

class Yoshi : public Avatar {
 public:
  template <typename... Args>
  Yoshi(Args&&... args) : Avatar(IID_YOSHI, std::forward<Args>(args)...) {}
};

class Square : public Actor {
 protected:
  bool m_hasActivatedOnPeach;
  bool m_hasActivatedOnYoshi;

  template <typename... Args>
  Square(int imageID, Args&&... args)
      : Actor(imageID, std::forward<Args>(args)...),
        m_hasActivatedOnPeach(false),
        m_hasActivatedOnYoshi(false) {
  }  // TODO: Make sure the first square doesn't activate on Peach and Yoshi
 public:
  bool isOn(Avatar* avatar) {
    return avatar->getX() == getX() && avatar->getY() == getY();
  }
  // Calling effect does not consider whether or not the square's effects are
  // for transient avatars or one that finish their roll there
  virtual void update(StudentWorld* world) {
    if (!isAlive()) {
      return;
    }
    if (isOn(world->getPeach())) {
      if (!m_hasActivatedOnPeach) {
        effect(world, world->getPeach());
      }
    } else {
      m_hasActivatedOnPeach = false;
    }

    if (isOn(world->getYoshi())) {
      if (!m_hasActivatedOnYoshi) {
        effect(world, world->getYoshi());
      }
    } else {
      m_hasActivatedOnYoshi = false;
    }
  }
  virtual void effect(StudentWorld* world, Avatar* avatar) = 0;
};

class CoinSquare : public Square {
 private:
  const int m_coins;

 protected:
  template <typename... Args>
  CoinSquare(int imageID, Args&&... args)
      : Square(imageID,
               std::forward<Args>(args)...,
               GraphObject::right,
               1,
               1.0),
        m_coins(imageID == IID_BLUE_COIN_SQUARE ? 3 : -3) {}

 public:
  virtual void effect(StudentWorld* world, Avatar* avatar) {
    avatar->addCoins(m_coins);
    if (m_coins > 0) {
      world->playSound(SOUND_GIVE_COIN);
    } else {
      world->playSound(SOUND_TAKE_COIN);
    }
  };
};

class BlueCoinSquare : public CoinSquare {
 public:
  template <typename... Args>
  BlueCoinSquare(Args&&... args)
      : CoinSquare(IID_BLUE_COIN_SQUARE, std::forward<Args>(args)...) {}

  virtual void die(StudentWorld* world) {}
};

class RedCoinSquare : public CoinSquare {
 public:
  template <typename... Args>
  RedCoinSquare(Args&&... args)
      : CoinSquare(IID_RED_COIN_SQUARE, std::forward<Args>(args)...) {}

  virtual void die(StudentWorld* world) {}
};

#endif  // ACTOR_H_
