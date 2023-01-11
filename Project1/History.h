#pragma once
// #ifndef HISTORY_HEADER
// #define HISTORY_HEADER

class History
{
  public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
};

// #endif