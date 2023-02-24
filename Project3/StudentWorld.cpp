#include "StudentWorld.h"
#include <string>
#include "Board.h"
#include "GameConstants.h"
using namespace std;

std::string operator+(std::string& s, int x) {
    return s + std::to_string(x);
}

GameWorld* createStudentWorld(string assetPath) {
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
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

    m_board.loadBoard(assetPath());

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            const Board::GridEntry entry = m_board.getContentsOf(x, y);
            switch (entry) {
                case Board::GridEntry::player:
                    m_actors.push_back(new BlueCoinSquare(x, y));
                    m_peach = new Peach(x, y);
                    m_yoshi = new Yoshi(x, y);
                    break;
                case Board::GridEntry::blue_coin_square:
                    m_actors.push_back(new BlueCoinSquare(x, y));
                    break;
                case Board::GridEntry::red_coin_square:
                    m_actors.push_back(new RedCoinSquare(x, y));
                    break;
                case Board::GridEntry::up_dir_square:
                    break;
                case Board::GridEntry::down_dir_square:
                    break;
                case Board::GridEntry::left_dir_square:
                    break;
                case Board::GridEntry::right_dir_square:
                    break;
                case Board::GridEntry::event_square:
                    break;
                case Board::GridEntry::bank_square:
                    break;
                case Board::GridEntry::star_square:
                    break;
                case Board::GridEntry::bowser:
                    break;
                case Board::GridEntry::boo:
                    break;
            }
        }
    }

    startCountdownTimer(99);

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // This code is here merely to allow the game to build, run, and terminate
    // after you hit ESC. Notice that the return value GWSTATUS_NOT_IMPLEMENTED
    // will cause our framework to end the game.

    for (Actor* const a : m_actors) {
        a->update(this);
    }

    setGameStatText("P1 Roll: "s + std::to_string(m_peach->remainingSteps()) +
                    " Stars: " + std::to_string(m_peach->getStars()) +
                    " $$: " + std::to_string(m_peach->getCoins()) + " " +
                    (m_peach->hasVortex() ? "VOR" : "") +
                    " | Time: " + std::to_string(timeRemaining()) +
                    " | Bank: " + std::to_string(getBank()) +
                    " | P2 Roll: " + std::to_string(m_yoshi->remainingSteps()) +
                    " Stars: " + std::to_string(m_yoshi->getStars()) +
                    " $$: " + std::to_string(m_yoshi->getCoins()) + " " +
                    (m_yoshi->hasVortex() ? "VOR" : ""));

    if (timeRemaining() <= 0) {
        return GWSTATUS_NOT_IMPLEMENTED;
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

int StudentWorld::addCoins(int coins) {
    m_bank += coins;
}

bool StudentWorld::canMoveInDirection(int x, int y, int direction) const {
    switch (direction) {
        case GraphObject::right:
            return canMoveRight(x, y);
            break;
        case GraphObject::left:
            return canMoveLeft(x, y);
            break;
        case GraphObject::up:
            return canMoveUp(x, y);
            break;
        case GraphObject::down:
            return canMoveDown(x, y);
            break;
        default:
            return false;
    }
}