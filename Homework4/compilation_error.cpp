#include "Set.h"  // class template from problem 2
#include <string>
using namespace std;

class Coord {
   public:
    Coord(int r, int c) : m_row(r), m_col(c) {}
    Coord() : m_row(0), m_col(0) {}
    double r() const { return m_row; }
    double c() const { return m_col; }

   private:
    double m_row;
    double m_col;
};

int main() {
    Set<int> si;
    si.insert(7);  // OK
    Set<string> ss;
    ss.insert("7-Up");  // OK
    Set<Coord> sc;
    sc.insert(Coord(7, -7));  // error!
}