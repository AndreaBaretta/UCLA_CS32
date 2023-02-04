#include <iostream>
#include <string>
#include <queue>

using namespace std;

class Coord {
   public:
    Coord(int r, int c) : m_row(r), m_col(c) {}
    int r() const { return m_row; }
    int c() const { return m_col; }

   private:
    int m_row;
    int m_col;
};

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[],
                int nRows,
                int nCols,
                int sr,
                int sc,
                int er,
                int ec) {
    queue<Coord> coords;
    coords.emplace(sr, sc);
    maze[sr][sc] = '0';
    while (!coords.empty()) {
        Coord coord = coords.front();
        coords.pop();
        int r = coord.r(), c = coord.c();
        cerr << "Explored coord: (" << r << "," << c << ")" << endl;
        if (r == er && c == ec) { return true; }
        if (maze[r][c+1] != '0' && maze[r][c+1] != 'X') { coords.emplace(r, c+1); maze[r][c+1] = '0'; }
        if (maze[r-1][c] != '0' && maze[r-1][c] != 'X') { coords.emplace(r-1, c); maze[r-1][c] = '0'; }
        if (maze[r][c-1] != '0' && maze[r][c-1] != 'X') { coords.emplace(r, c-1); maze[r][c-1] = '0'; }
        if (maze[r+1][c] != '0' && maze[r+1][c] != 'X') { coords.emplace(r+1, c); maze[r+1][c] = '0'; }
    }
    return false;
}

int main() {
    string maze[10] = {
        "XXXXXXXXXX",
        "X..X...X.X",
        "X.XXXX.X.X",
        "X.X.X..X.X",
        "X...X.XX.X",
        "XXX......X",
        "X.X.XXXX.X",
        "X.XXX....X",
        "X...X..X.X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10,10, 5,3, 8,8)) {
        cout << "Solvable!" << endl;
    } else {
        cout << "Out of luck!" << endl;
    }

    string maze2[10] = {
        "XXXXX",
        "X.x.X",
        "XXXXX"
    };
    cout << pathExists(maze2, 3,5, 1,1, 1,3) << endl;
}
