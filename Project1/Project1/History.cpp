#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	this->m_rows = nRows;
	this->m_cols = nCols;
	//fill the history grid with 0s for the given dimensions
	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			m_grid[i][j] = 0;
		}
	}
}

bool History::record(int r, int c) {
	if (r > m_rows || c > m_cols || r < 1|| c < 1)
		return false;
	else {
		m_grid[r-1][c-1]++;
		return true;
	}
}

void History::display() const {
	clearScreen();
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			if (m_grid[i][j] == 0) {
				cout << '.';
			}
			else if (m_grid[i][j] > 0 && m_grid[i][j] <= 25) {
				cout << (char)('A' - 1 + m_grid[i][j]);
			}
			else
				cout << 'Z';
		}
		cout << endl;
	}
	cout << endl;
}
