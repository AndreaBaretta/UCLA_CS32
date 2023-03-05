#include "Actor.h"
#include <utility>
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and
// StudentWorld.cpp

// -------- ACTOR --------

void Actor::die(StudentWorld* world) {
  m_alive = false;
}

bool Actor::isImpactible() const {
  return false;
}

void Actor::impact() {}

bool Actor::isAlive() const {
  return m_alive;
}

// -------- AVATAR --------

Avatar::Avatar(int imageID, int x, int y)
    : Actor(imageID, x, y, GraphObject::right, 0, 1.0),
      m_ticks_to_move(0),
      m_state(WAITING_TO_ROLL),
      m_walk_direction(WalkDirection::RIGHT),
      m_coins(0),
      m_stars(0),
      m_has_vortex(false) {}

void Avatar::update(StudentWorld* world) {
  switch (m_state) {
    case WAITING_TO_ROLL: {
      int action = getAction(world);
      int die_roll = randInt(1, 10);
      // int die_roll = 5;
      switch (action) {
        case ACTION_ROLL:
          m_ticks_to_move = 8 * die_roll;
          m_state = WALKING;
          break;
        case ACTION_FIRE:
          if (hasVortex()) {
            auto [d_x, d_y] = MOVES[(int)m_walk_direction];
            world->addActor(new Vortex(m_walk_direction, d_x * 8, d_y * 8));
            world->playSound(SOUND_PLAYER_FIRE);
            m_has_vortex = false;
          }
        default:
          return;
      }

      break;
    }
    case WALKING: {
      if (m_ticks_to_move % 8 == 0) {
        if (world->isAtFork(getX() / 16, getY() / 16, m_walk_direction)) {
          int action = getAction(world);
          if (action == ACTION_LEFT && world->canMoveLeft(getX(), getY())) {
            m_walk_direction = WalkDirection::LEFT;
          } else if (action == ACTION_RIGHT &&
                     world->canMoveRight(getX(), getY())) {
            m_walk_direction = WalkDirection::RIGHT;
          } else if (action == ACTION_UP && world->canMoveUp(getX(), getY())) {
            m_walk_direction = WalkDirection::UP;
          } else if (action == ACTION_DOWN &&
                     world->canMoveDown(getX(), getY())) {
            m_walk_direction = WalkDirection::DOWN;
          }
        }
      }
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
bool Avatar::isWalking() const {
  return m_state == WALKING;
}
void Avatar::setWalkDirection(WalkDirection direction) {
  m_walk_direction = direction;
}
void Avatar::teleport(int new_x, int new_y) {
  moveTo(new_x, new_y);
}
void Avatar::swap(Avatar* other) {
// i. x, y coordinates
// ii. the number of ticks left that the player has to move before
// completing their roll
// iii. the player's walk direction
// iv. the player's sprite direction
// v. the player's roll/walk state
  int tmp_x, tmp_y, tmp_ticks_to_move, tmp_direction;
  State tmp_state;
  WalkDirection tmp_walk_direction;
  tmp_x = other->getX();
  tmp_y = other->getY();
  tmp_ticks_to_move = other->m_ticks_to_move;
  tmp_direction = other->getDirection();
  tmp_state = other->m_state;
  tmp_walk_direction = other->m_walk_direction;

  other->moveTo(getX(), getY());
  other->m_ticks_to_move = m_ticks_to_move;
  other->setDirection(getDirection());
  other->m_state = m_state;
  other->m_walk_direction = m_walk_direction;

  moveTo(tmp_x, tmp_y);
  m_ticks_to_move = tmp_ticks_to_move;
  setDirection(tmp_direction);
  m_state = tmp_state;
  m_walk_direction = tmp_walk_direction;
}

// -------- PEACH --------

Peach::Peach(int x, int y) : Avatar(IID_PEACH, x, y) {}

int Peach::getAction(StudentWorld* world) {
  return world->getAction(1);
}

// -------- YOSHI --------

Yoshi::Yoshi(int x, int y) : Avatar(IID_YOSHI, x, y) {}

int Yoshi::getAction(StudentWorld* world) {
  return world->getAction(2);
}

// -------- SQUARE --------

Square::Square(int imageID, int x, int y)
    : Actor(imageID, x, y, GraphObject::right, 1, 1.0),
      m_hasActivatedOnPeach(false),
      m_hasActivatedOnYoshi(false) {}

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

void Square::deactivate() {
  m_hasActivatedOnYoshi = true;
  m_hasActivatedOnPeach = true;
}

// -------- BLUE COIN SQUARE --------

BlueCoinSquare::BlueCoinSquare(int x, int y)
    : Square(IID_BLUE_COIN_SQUARE, x, y) {}

void BlueCoinSquare::effect(StudentWorld* world, Avatar* avatar) {
  if (avatar->remainingSteps() != 0) {
    return;
  }
  avatar->addCoins(3);
  world->playSound(SOUND_GIVE_COIN);
}

void BlueCoinSquare::die(StudentWorld* world) {
  world->addActor(new RedCoinSquare(getX(), getY()));
  Square::die(world);
}

// -------- RED COIN SQUARE --------

RedCoinSquare::RedCoinSquare(int x, int y)
    : Square(IID_RED_COIN_SQUARE, x, y) {}

void RedCoinSquare::effect(StudentWorld* world, Avatar* avatar) {
  if (avatar->remainingSteps() != 0) {
    return;
  }
  avatar->addCoins(-3);
  world->playSound(SOUND_TAKE_COIN);
}

// -------- STAR SQUARE --------

StarSquare::StarSquare(int x, int y) : Square(IID_STAR_SQUARE, x, y) {}

void StarSquare::effect(StudentWorld* world, Avatar* avatar) {
  if (avatar->getCoins() >= 20) {
    avatar->addCoins(-20);
    avatar->addStars(1);
    world->playSound(SOUND_GIVE_STAR);
  }
}

// -------- DIRECTIONAL SQUARE --------

DirectionalSquare::DirectionalSquare(int imageID, int x, int y, int direction)
    : Square(imageID, x, y) {
  setDirection(direction);
  if (direction == GraphObject::right) {
    m_force = WalkDirection::RIGHT;
  }
  if (direction == GraphObject::left) {
    m_force = WalkDirection::LEFT;
  }
  if (direction == GraphObject::up) {
    m_force = WalkDirection::UP;
  }
  if (direction == GraphObject::down) {
    m_force = WalkDirection::DOWN;
  }
}

void DirectionalSquare::effect(StudentWorld* world, Avatar* avatar) {
  avatar->setWalkDirection(m_force);
}

// -------- BANK SQUARE --------

BankSquare::BankSquare(int x, int y) : Square(IID_BANK_SQUARE, x, y) {}

void BankSquare::effect(StudentWorld* world, Avatar* avatar) {
  if (avatar->remainingSteps() != 0) {
    int coins = randInt(1, std::min(5, avatar->getCoins()));
    world->addToBank(coins);
    avatar->addCoins(-coins);
    world->playSound(SOUND_DEPOSIT_BANK);
  } else {
    avatar->addCoins(world->getBank());
    world->clearBank();
    world->playSound(SOUND_WITHDRAW_BANK);
  }
}

// -------- EVENT SQUARE --------

EventSquare::EventSquare(int x, int y) : Square(IID_EVENT_SQUARE, x, y) {}

void EventSquare::effect(StudentWorld* world, Avatar* avatar) {
  int event = randInt(1, 3);
  switch (event) {
    case 1:

      break;
    case 2:
      break;
    case 3:
      break;
  }
}

// -------- VORTEX --------

void Vortex::update(StudentWorld* world) {
  if (!isAlive()) {
    return;
  }
  auto [d_x, d_y] = MOVES[(int)m_walk_direction];
  moveTo(getX() + d_x, getY() + d_y);
  if (getX() >= VIEW_WIDTH || getY() <= VIEW_HEIGHT) {
    die(world);
  }
  if (world->checkCollision(this)) {
    die(world);
    world->playSound(SOUND_HIT_BY_VORTEX);
  }
}