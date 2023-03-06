#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <string>
#include <list>
#include "Board.h"
#include "GameWorld.h"
#include "GraphObject.h"

enum class WalkDirection { RIGHT = 0, LEFT = 1, UP = 2, DOWN = 3 };

class Peach;
class Yoshi;
class Actor;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
 private:
  Board m_board;
  Peach* m_peach;
  Yoshi* m_yoshi;
  std::list<Actor*> m_actors;
  int m_bank;

 public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  int getBank() const;
  void clearBank();
  void addToBank(int coins);
  bool canMoveInDirection(int x, int y, WalkDirection direction);
  bool canMoveRight(int x, int y);
  bool canMoveLeft(int x, int y);
  bool canMoveUp(int x, int y);
  bool canMoveDown(int x, int y);
  Peach* getPeach() const;
  Yoshi* getYoshi() const;
  bool checkCollision(Actor* actor);
  void addActor(Actor* actor);
  bool isAtFork(int x, int y, WalkDirection direction);
  Actor* getSquare(int x, int y);
  Actor* getRandomSquare();
  std::vector<WalkDirection> validDirections(int x, int y);
};

#endif  // STUDENTWORLD_H_
