#include <iostream>
#include <queue>
#include <cassert>
using namespace std;
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

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	queue <Coord> coordStack;
	coordStack.push(Coord(sr, sc));
	maze[sr][sc] = '!'; //let '! denote a found space
	while (!coordStack.empty()) {
		Coord top = coordStack.front();
		coordStack.pop();
		Coord * current = &top;
		cerr << current->r() << " , " << current->c() << endl;
		if (current->r() == er && current->c() == ec) {
			return true;
		}
		//check space to the south
		if (maze[current->r() + 1][current->c()] == '.') {
			maze[current->r() + 1][current->c()] = '!';
			coordStack.push(Coord(current->r() + 1, current->c()));
		}
		//check space to the west
		if (maze[current->r()][current->c() - 1] == '.') {
			maze[current->r()][current->c() - 1] = '!';
			coordStack.push(Coord(current->r(), current->c() - 1));
		}
		//check space to the north
		if (maze[current->r() - 1][current->c()] == '.') {
			maze[current->r() - 1][current->c()] = '!';
			coordStack.push(Coord(current->r() - 1, current->c()));
		}
		//check space to the east
		if (maze[current->r()][current->c() + 1] == '.') {
			maze[current->r()][current->c() + 1] = '!';
			coordStack.push(Coord(current->r(), current->c() + 1));
		}
	}
	return false;
}

int main() {
	string maze[10] = {
				"XXXXXXXXXX",
				"X....X...X",
				"X.XX.XX..X",
				"XXX....X.X",
				"X.XXX.XXXX",
				"X.X...X..X",
				"X...X.X..X",
				"XXXXX.X.XX",
				"X........X",
				"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 3, 5, 8, 8))
		cout << "Yay!" << endl;
	else {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				cout << maze[i][j];
			}
			cout << endl;
		}
	}
}