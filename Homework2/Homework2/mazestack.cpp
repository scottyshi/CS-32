#include <stack>
#include <iostream>
#include <string>

using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	stack<Coord> carey; //declare a stack of Coord
	Coord s(sr, sc);//start coord
	Coord f(er, ec);//finish coord
	carey.push(s);
	maze[sr][sc] = '!';//! means that it's been discovered
	while (!carey.empty())
	{
		Coord x = carey.top();
		carey.pop();
		if (x.c() == f.c() && x.r() == f.r())
			return true;
		if (x.r() - 1 >= 0 && maze[x.r() - 1][x.c()] == '.')
		{
			Coord add(x.r() - 1, x.c());
			carey.push(add);
			maze[x.r() - 1][x.c()] = '!';
		}
		if (x.c() + 1 < nCols && maze[x.r()][x.c() + 1] == '.')
		{
			Coord add(x.r(), x.c() + 1);
			carey.push(add);
			maze[x.r()][x.c() + 1] = '!';
		}
		if (x.r() + 1 < nRows && maze[x.r() + 1][x.c()] == '.')
		{
			Coord add(x.r() + 1, x.c());
			carey.push(add);
			maze[x.r() + 1][x.c()] = '!';
		}
		if (x.c() - 1 >= 0 && maze[x.r()][x.c()-1] == '.')
		{
			Coord add(x.r(), x.c()-1);
			carey.push(add);
			maze[x.r()][x.c() - 1] = '!';
		}
	}
	return false;
}
