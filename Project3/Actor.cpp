#include "Actor.h"
#include <utility>
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and
// StudentWorld.cpp

// -------- ACTOR --------

void Actor::die(StudentWorld* world) {}

bool Actor::isAlive() {
  return m_alive;
}

// -------- AVATAR --------

void Avatar::update(StudentWorld* world) {
  switch (m_state) {
    case WAITING_TO_ROLL: {
      int action = getAction(world);
      int die_roll = rand() % 10 + 1;
      // int die_roll = 5;
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
      // std::cout << "x: " << getX() << ", y: " << getY() << std::endl;
      if (!world->canMoveInDirection(getX() / 16, getY() / 16,
                                     m_walk_direction) &&
          m_ticks_to_move % 8 == 0) {
        if (m_walk_direction == WalkDirection::RIGHT ||
            m_walk_direction == WalkDirection::LEFT) {
          m_walk_direction = world->canMoveUp(getX() / 16, getY() / 16)
                                 ? WalkDirection::UP
                                 : WalkDirection::DOWN;
        } else {
          m_walk_direction = world->canMoveRight(getX() / 16, getY() / 16)
                                 ? WalkDirection::RIGHT
                                 : WalkDirection::LEFT;
        }
      }
      if (m_walk_direction == WalkDirection::LEFT) {
        setDirection(GraphObject::left);
      } else {
        setDirection(GraphObject::right);
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
int Avatar::remainingSteps() {
  return (m_ticks_to_move + 7) / 8;
}
int Avatar::getCoins() {
  return m_coins;
}
int Avatar::getStars() {
  return m_stars;
}
bool Avatar::hasVortex() {
  return m_has_vortex;
}
void Avatar::addCoins(int coins) {
  m_coins += coins;
}
void Avatar::addStars(int stars) {
  m_stars += stars;
  m_stars = std::max(m_stars, 0);
}
void Avatar::giveVortex() {
  m_has_vortex = true;
}
const bool Avatar::isWalking() const {
  return m_state == WALKING;
}

// -------- PEACH --------

int Peach::getAction(StudentWorld* world) {
  return world->getAction(1);
}

// -------- YOSHI --------

int Yoshi::getAction(StudentWorld* world) {
  return world->getAction(2);
}

// -------- SQUARE --------

bool Square::isOn(Avatar* avatar) {
  return avatar->getX() == getX() && avatar->getY() == getY();
}

void Square::update(StudentWorld* world) {
  if (!isAlive()) {
    return;
  }
  if (isOn(world->getPeach())) {
    if (!m_hasActivatedOnPeach) {
      effect(world, world->getPeach());
      m_hasActivatedOnPeach = true;
    }
  } else {
    m_hasActivatedOnPeach = false;
  }

  if (isOn(world->getYoshi())) {
    if (!m_hasActivatedOnYoshi) {
      effect(world, world->getYoshi());
      m_hasActivatedOnYoshi = true;
    }
  } else {
    m_hasActivatedOnYoshi = false;
  }
}

// -------- BLUE COIN SQUARE --------

void BlueCoinSquare::effect(StudentWorld* world, Avatar* avatar) {
  if (avatar->remainingSteps() != 0) {
    return;
  }
  avatar->addCoins(3);
  world->playSound(SOUND_GIVE_COIN);
}

void BlueCoinSquare::die(StudentWorld* world) {}

// -------- RED COIN SQUARE --------

void RedCoinSquare::effect(StudentWorld* world, Avatar* avatar) {
  if (avatar->remainingSteps() != 0) {
    return;
  }
  avatar->addCoins(-3);
  world->playSound(SOUND_TAKE_COIN);
}