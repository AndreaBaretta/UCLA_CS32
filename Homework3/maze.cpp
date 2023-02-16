// #include <iostream>
// #include <string>
// #include <stack>

// using namespace std;

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[],
                int nRows,
                int nCols,
                int sr,
                int sc,
                int er,
                int ec) {
    if (sr == er && sc == ec) { return true; }
    maze[sr][sc] = '0';

    // cerr << "Explored coord: (" << r << "," << c << ")" << endl;
    if (maze[sr][sc+1] != '0' && maze[sr][sc+1] != 'X') { if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec)) { return true; } }
    if (maze[sr-1][sc] != '0' && maze[sr-1][sc] != 'X') { if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec)) { return true; } }
    if (maze[sr][sc-1] != '0' && maze[sr][sc-1] != 'X') { if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec)) { return true; } }
    if (maze[sr+1][sc] != '0' && maze[sr+1][sc] != 'X') { if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec)) { return true; } }

    return false;
}

// If the start location is equal to the ending location, then we've
//     solved the maze, so return true.
// Mark the start location as visted.
// For each of the four directions,
//     If the location one step in that direction (from the start
//         location) is unvisited,
//             then call pathExists starting from that location (and
//                         ending at the same ending location as in the
//                         current call).
//                     If that returned true,
//                         then return true.
// Return false.

// int test_routine() {
//     string maze[10] = {
//         "XXXXXXXXXX",
//         "X..X...X.X",
//         "X.XXXX.X.X",
//         "X.X.X..X.X",
//         "X...X.XX.X",
//         "XXX......X",
//         "X.X.XXXX.X",
//         "X.XXX....X",
//         "X...X..X.X",
//         "XXXXXXXXXX"
//     };

//     if (pathExists(maze, 10,10, 5,3, 8,8)) {
//         cerr << "Solvable!" << endl;
//     } else {
//         cerr << "Out of luck!" << endl;
//     }
// }
