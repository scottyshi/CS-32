#include <string>

using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{//I want to keep er ec constant while changing sr and sc
	if (sr == er && sc == ec)
		return true;
	maze[sr][sc] = '!';
	if (sr + 1 < nRows && maze[sr + 1][sc] == '.')
		if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
			return true;
	if (sr - 1 >= 0 && maze[sr - 1][sc] == '.')
		if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
			return true;
	if (sc + 1 < nCols && maze[sr][sc + 1] == '.')
		if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
			return true;
	if (sc - 1 >= 0 && maze[sr][sc - 1] == '.')
		if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
			return true;
	return false;
}
