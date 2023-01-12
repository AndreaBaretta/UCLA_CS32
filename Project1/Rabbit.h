#pragma once
// #ifndef RABBIT_HEADER
// #define RABBIT_HEADER

#include "Arena.h"

class Arena;

class Rabbit  // This is needed timplementation file named utilities.cpp, and place
{
  public:
      // Constructor
    Rabbit(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    int    m_idleTurnsRemaining;
};

// #endif