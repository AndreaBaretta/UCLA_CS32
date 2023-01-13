#ifndef GAME_HEADER
#define GAME_HEADER

#include <string>
#include "Arena.h"

using namespace std;

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nRabbits);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();

};

#endif