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

bool Actor::isGridSquare() const {
  return false;
}

bool Actor::isAlive() const {
  return m_alive;
}

std::pair<int, int> Actor::getPosition() const {
  return {getX(), getY()};
}

// -------- AVATAR --------

Avatar::Avatar(int imageID, int x, int y)
    : Actor(imageID, x, y, GraphObject::right, 0, 1.0),
      m_ticks_to_move(0),
      m_state(WAITING_TO_ROLL),
      m_walk_direction(WalkDirection::RIGHT),
      m_coins(0),
      m_stars(0),
      m_has_vortex(false),
      m_teleported(false),
      m_swapped(false),
      m_has_moved(false),
      m_been_forced(false) {}

void Avatar::update(StudentWorld* world) {
  switch (m_state) {
    case WAITING_TO_ROLL: {
      if (m_teleported) {
        std::vector<WalkDirection> valid_directions(4);
        if (world->canMoveUp(getX(), getY())) {
          valid_directions.push_back(WalkDirection::UP);
        }
        if (world->canMoveDown(getX(), getY())) {
          valid_directions.push_back(WalkDirection::DOWN);
        }
        if (world->canMoveRight(getX(), getY())) {
          valid_directions.push_back(WalkDirection::RIGHT);
        }
        if (world->canMoveLeft(getX(), getY())) {
          valid_directions.push_back(WalkDirection::LEFT);
        }
        m_walk_direction =
            valid_directions[randInt(1, valid_directions.size() - 1)];
        m_teleported = false;
      }
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
            world->addActor(new Vortex(m_walk_direction, getX() + d_x * 8,
                                       getY() + d_y * 8));
            world->playSound(SOUND_PLAYER_FIRE);
            m_has_vortex = false;
          }
        default:
          return;
      }

      break;
    }
    case WALKING: {
      if (m_ticks_to_move % 8 == 0 && !m_been_forced && !m_teleported) {
        if (world->isAtFork(getX() / 16, getY() / 16, m_walk_direction)) {
          int action = getAction(world);
          if (action == ACTION_LEFT && world->canMoveLeft(getX() / 16, getY() / 16)) {
            m_walk_direction = WalkDirection::LEFT;
          } else if (action == ACTION_RIGHT &&
                     world->canMoveRight(getX() / 16, getY() / 16)) {
            m_walk_direction = WalkDirection::RIGHT;
          } else if (action == ACTION_UP && world->canMoveUp(getX() / 16, getY() / 16)) {
            m_walk_direction = WalkDirection::UP;
          } else if (action == ACTION_DOWN &&
                     world->canMoveDown(getX() / 16, getY() / 16)) {
            m_walk_direction = WalkDirection::DOWN;
          } else if (m_has_moved) {
            return;
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
      m_has_moved = true;
      m_swapped = false;
      m_been_forced = false;
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
void Avatar::forceDirection(WalkDirection direction) {
  m_walk_direction = direction;
  m_been_forced = true;
}
void Avatar::teleport(int new_x, int new_y) {
  moveTo(new_x, new_y);
  m_teleported = true;
}
void Avatar::swapPosition(Avatar* other) {
  std::swap(m_ticks_to_move, other->m_ticks_to_move);
  std::swap(m_state, other->m_state);
  std::swap(m_walk_direction, other->m_walk_direction);
  std::swap(m_teleported, other->m_teleported);
  std::swap(m_has_moved, other->m_has_moved);
  std::swap(m_been_forced, other->m_been_forced);
  int tmp_x, tmp_y, tmp_direction;
  tmp_x = other->getX();
  tmp_y = other->getY();

  other->moveTo(getX(), getY());
  other->setDirection(getDirection());

  moveTo(tmp_x, tmp_y);
  setDirection(tmp_direction);
  m_swapped = false;
  other->m_swapped = true;
}
bool Avatar::beenSwapped() const {
  return m_swapped;
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
  m_hasActivatedOnPeach = true;
  m_hasActivatedOnYoshi = true;
}

bool Square::isGridSquare() const {
  return true;
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

DirectionalSquare::DirectionalSquare(int x, int y, int direction)
    : Square(IID_DIR_SQUARE, x, y) {
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
  avatar->forceDirection(m_force);
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
  if (avatar->beenSwapped() || avatar->remainingSteps() != 0) {
    return;
  }
  int event = randInt(1, 3);
  switch (event) {
    case 1: {
      auto [x, y] = world->getRandomSquare()->getPosition();
      avatar->teleport(x, y);
      world->playSound(SOUND_PLAYER_TELEPORT);
      break;
    }
    case 2:
      avatar->swapPosition((avatar == world->getPeach()
                                ? (Avatar*)world->getYoshi()
                                : (Avatar*)world->getPeach()));
      world->playSound(SOUND_PLAYER_TELEPORT);
      break;
    case 3:
      avatar->giveVortex();
      break;
  }
}

// -------- DROPPING SQUARE --------

DroppingSquare::DroppingSquare(int x, int y)
    : Square(IID_DROPPING_SQUARE, x, y) {}

void DroppingSquare::effect(StudentWorld* world, Avatar* avatar) {
  if (avatar->remainingSteps() != 0) {
    return;
  }
  int action = randInt(1, 2);
  switch (action) {
    case 1:
      avatar->addCoins(std::max(-10, -avatar->getCoins()));
      break;
    case 2:
      avatar->addStars(std::max(-1, -avatar->getCoins()));
      break;
  }
  world->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
}

// -------- VORTEX --------

void Vortex::update(StudentWorld* world) {
  if (!isAlive()) {
    return;
  }
  auto [d_x, d_y] = MOVES[(int)m_walk_direction];
  moveTo(getX() + d_x, getY() + d_y);
  if (getX() <= 0 || getY() <= 0 || getX() >= VIEW_WIDTH ||
      getY() >= VIEW_HEIGHT) {
    die(world);
  }
  if (world->checkCollision(this)) {
    die(world);
    world->playSound(SOUND_HIT_BY_VORTEX);
  }
}