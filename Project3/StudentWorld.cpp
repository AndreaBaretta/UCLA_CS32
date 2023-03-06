#include "StudentWorld.h"
#include <iostream>
#include <string>
#include "Actor.h"
#include "Board.h"
#include "GameConstants.h"

const int GraphObject::left;
const int GraphObject::right;
const int GraphObject::up;
const int GraphObject::down;

GameWorld* createStudentWorld(std::string assetPath) {
  return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(std::string assetPath)
    : GameWorld(assetPath),
      m_board(),
      m_peach(nullptr),
      m_yoshi(nullptr),
      m_actors(),
      m_bank(0) {}

int StudentWorld::init() {
  // startCountdownTimer(5);  // this placeholder causes timeout after 5
  // seconds

  // empty, player, blue_coin_square, red_coin_square, up_dir_square,
  // down_dir_square, left_dir_square, right_dir_square, event_square,
  // bank_square, star_square, bowser, boo

  m_board.loadBoard(assetPath() + "board0" + std::to_string(getBoardNumber()) +
                    ".txt");

  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      const int x = SPRITE_WIDTH * i;
      const int y = SPRITE_HEIGHT * j;
      const Board::GridEntry entry = m_board.getContentsOf(i, j);
      switch (entry) {
        case Board::GridEntry::player:
          m_peach = new Peach(x, y);
          m_yoshi = new Yoshi(x, y);
          m_actors.push_back(m_peach);
          m_actors.push_back(m_yoshi);
          m_actors.push_back(new BlueCoinSquare(x, y));
          break;
        case Board::GridEntry::blue_coin_square:
          m_actors.push_back(new BlueCoinSquare(x, y));
          break;
        case Board::GridEntry::red_coin_square:
          m_actors.push_back(new RedCoinSquare(x, y));
          break;
        case Board::GridEntry::up_dir_square:
          m_actors.push_back(new DirectionalSquare(x, y, GraphObject::up));
          break;
        case Board::GridEntry::down_dir_square:
          m_actors.push_back(new DirectionalSquare(x, y, GraphObject::down));
          break;
        case Board::GridEntry::left_dir_square:
          m_actors.push_back(new DirectionalSquare(x, y, GraphObject::left));
          break;
        case Board::GridEntry::right_dir_square:
          m_actors.push_back(new DirectionalSquare(x, y, GraphObject::right));
          break;
        case Board::GridEntry::event_square:
          m_actors.push_back(new EventSquare(x, y));
          break;
        case Board::GridEntry::bank_square:
          m_actors.push_back(new BankSquare(x, y));
          break;
        case Board::GridEntry::star_square:
          m_actors.push_back(new StarSquare(x, y));
          break;
        case Board::GridEntry::bowser:
          m_actors.push_back(new Bowser(x, y));
          m_actors.push_back(new BlueCoinSquare(x, y));
          break;
        case Board::GridEntry::boo:
          m_actors.push_back(new Boo(x, y));
          m_actors.push_back(new BlueCoinSquare(x, y));
          break;
        default:
          break;
      }
    }
  }

  startCountdownTimer(9999);

  return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
  // This code is here merely to allow the game to build, run, and terminate
  // after you hit ESC. Notice that the return value GWSTATUS_NOT_IMPLEMENTED
  // will cause our framework to end the game.

  // for (Actor* const a : m_actors) {
  //   a->update(this);
  // }

  for (std::list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end();) {
    (*it)->update(this);
    ++it;
  }

  for (std::list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end();) {
    if (!(*it)->isAlive()) {
      (*it)->die();
      delete *it;
      it = m_actors.erase(it);
    } else {
      ++it;
    }
  }

  setGameStatText(std::string("P1 Roll: ") + std::to_string(m_peach->remainingSteps()) +
                  " Stars: " + std::to_string(m_peach->getStars()) +
                  " $$: " + std::to_string(m_peach->getCoins()) +
                  (m_peach->hasVortex() ? " VOR" : "") +
                  " | Time: " + std::to_string(timeRemaining()) +
                  " | Bank: " + std::to_string(getBank()) +
                  " | P2 Roll: " + std::to_string(m_yoshi->remainingSteps()) +
                  " Stars: " + std::to_string(m_yoshi->getStars()) +
                  " $$: " + std::to_string(m_yoshi->getCoins()) + " " +
                  (m_yoshi->hasVortex() ? "VOR" : ""));

  if (timeRemaining() <= 0) {
    if (m_peach->getStars() > m_yoshi->getStars()) {
      return GWSTATUS_PEACH_WON;
    } else if (m_yoshi->getStars() > m_peach->getStars()) {
      return GWSTATUS_YOSHI_WON;
    } else {
      return m_peach->getCoins() > m_yoshi->getCoins() ? GWSTATUS_PEACH_WON
                                                       : GWSTATUS_YOSHI_WON;
    }
  }

  return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
  for (Actor* const a : m_actors) {
    delete a;
  }
}

int StudentWorld::getBank() const {
  return m_bank;
}

void StudentWorld::clearBank() {
  m_bank = 0;
}

void StudentWorld::addToBank(int coins) {
  m_bank += coins;
}

bool StudentWorld::canMoveInDirection(int x, int y, WalkDirection direction) {
  switch (direction) {
    case WalkDirection::RIGHT:
      return canMoveRight(x, y);
      break;
    case WalkDirection::LEFT:
      return canMoveLeft(x, y);
      break;
    case WalkDirection::UP:
      return canMoveUp(x, y);
      break;
    case WalkDirection::DOWN:
      return canMoveDown(x, y);
      break;
    default:
      return false;
  }
}

bool StudentWorld::canMoveRight(int x, int y) {
  // std::cout << "Grid to right of " << x << ", " << y << " = " <<
  // m_board.getContentsOf(x, y) << ": " <<  m_board.getContentsOf(x + 1, y) <<
  // std::endl;
  return m_board.getContentsOf(x + 1, y) != Board::GridEntry::empty;
}

bool StudentWorld::canMoveLeft(int x, int y) {
  // std::cout << "Grid to left of " << x << ", " << y << " = " <<
  // m_board.getContentsOf(x, y) << ": " <<  m_board.getContentsOf(x - 1, y) <<
  // std::endl;
  return m_board.getContentsOf(x - 1, y) != Board::GridEntry::empty;
}

bool StudentWorld::canMoveUp(int x, int y) {
  // std::cout << "Grid to up of " << x << ", " << y << " = " <<
  // m_board.getContentsOf(x, y) << ": " <<  m_board.getContentsOf(x, y + 1) <<
  // std::endl;
  return m_board.getContentsOf(x, y + 1) != Board::GridEntry::empty;
}

bool StudentWorld::canMoveDown(int x, int y) {
  // std::cout << "Grid to down of " << x << ", " << y << " = " <<
  // m_board.getContentsOf(x, y) << ": " <<  m_board.getContentsOf(x, y - 1) <<
  // std::endl;
  return m_board.getContentsOf(x, y - 1) != Board::GridEntry::empty;
}

Peach* StudentWorld::getPeach() const {
  return m_peach;
}
Yoshi* StudentWorld::getYoshi() const {
  return m_yoshi;
}

bool StudentWorld::checkCollision(Actor* actor) {
  for (Actor* a : m_actors) {
    if (a->isImpactible() && (a->getX() < actor->getX() + SPRITE_WIDTH &&
                              a->getX() > actor->getX() - SPRITE_WIDTH &&
                              a->getY() < actor->getY() + SPRITE_HEIGHT &&
                              a->getY() > actor->getY() - SPRITE_HEIGHT)) {
      a->impact(this);
      return true;
    }
  }
  return false;
}

void StudentWorld::addActor(Actor* actor) {
  m_actors.push_back(actor);
}

bool StudentWorld::isAtFork(int x, int y, WalkDirection direction) {
  int i = 0;
  if (direction == WalkDirection::RIGHT) {
    if (canMoveDown(x, y)) { ++i; }
    if (canMoveUp(x, y)) { ++i; }
    if (canMoveRight(x, y)) { ++i; }
  } else if (direction == WalkDirection::LEFT) {
    if (canMoveDown(x, y)) { ++i; }
    if (canMoveUp(x, y)) { ++i; }
    if (canMoveLeft(x, y)) { ++i; }
  } else if (direction == WalkDirection::UP) {
    if (canMoveRight(x, y)) { ++i; }
    if (canMoveLeft(x, y)) { ++i; }
    if (canMoveUp(x, y)) { ++i; }
  } else if (direction == WalkDirection::DOWN) {
    if (canMoveRight(x, y)) { ++i; }
    if (canMoveLeft(x, y)) { ++i; }
    if (canMoveDown(x, y)) { ++i; }
  }

  return i >= 2;
}

Actor* StudentWorld::getSquare(int x, int y) {
  for (Actor* a : m_actors) {
    if (a->isGridSquare() && a->getX()/16 == x && a->getY()/16 == y) {
      return a;
    }
  }
  return nullptr;
}

Actor* StudentWorld::getRandomSquare() {
  int i;
  std::list<Actor*>::const_iterator it;
  do {
    i = randInt(0, m_actors.size() - 1);
    it = m_actors.cbegin();
    std::advance(it, i);
  } while (!(*it)->isGridSquare());
  return *it;
}

std::vector<WalkDirection> StudentWorld::validDirections(int x, int y) {
  std::vector<WalkDirection> valid_directions;
  if (canMoveUp(x, y)) {
    valid_directions.push_back(WalkDirection::UP);
  }
  if (canMoveDown(x, y)) {
    valid_directions.push_back(WalkDirection::DOWN);
  }
  if (canMoveRight(x, y)) {
    valid_directions.push_back(WalkDirection::RIGHT);
  }
  if (canMoveLeft(x, y)) {
    valid_directions.push_back(WalkDirection::LEFT);
  }
  return valid_directions;
}