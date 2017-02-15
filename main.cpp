#include <ncurses.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "Cell.h"

using vector = std::vector<Cell>;

std::vector< vector > matrix;

int main(int argc, char* argv[]) {

	// ncurses initialisations
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	start_color();
	
	init_color(COLOR_GREEN,100,100,100);
	init_pair(1, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(2, COLOR_RED   , COLOR_RED   );
	init_pair(3, COLOR_GREEN , COLOR_GREEN );

	int row,col;
	getmaxyx(stdscr,row,col);
	matrix.reserve(col);

	for (int x = 0; x < col; ++x) {
		vector v;
		matrix.push_back(v);
		matrix[x].reserve(row);
		for (int y = 0; y < row; ++y) {
			matrix[x].push_back(Cell());
			matrix[x][y].set(false);
			matrix[x][y].next(false);
		}
	}

	int ch;
	bool edit = true;
	int cursorx = 0, cursory = 0;
	int speed = 8;
	int iteration = 0;
	
	ed:
	while (edit) {
		nodelay(stdscr,FALSE);
		clear();
		for (int y = 0; y < row - 1; ++y) {
			for (int x = 0; x < col - 1; ++x) {
				if ((cursorx == x) && (cursory == y)) {
					attron (COLOR_PAIR(2));
					printw("@");
					attroff(COLOR_PAIR(2));
				} else {
					if (matrix[x][y].get()) {
						attron(COLOR_PAIR(1));
					} else {
						attron(COLOR_PAIR(3));
					}
					printw("%c",matrix[x][y].print());
				}
			}
			printw("\n");
		}
		attroff(COLOR_PAIR(1)); attroff(COLOR_PAIR(3));
		printw("Speed = %i",speed);
		printw("  Iteration = %i -EDIT-    (", iteration);
		printw("%i,",cursorx); printw("%i)",cursory);
		refresh();

		ch = getch();
		if (ch == KEY_UP) {
			if (cursory > 0) {
				cursory -= 1;
			}
		}
		if (ch == KEY_DOWN) {
			if (cursory < row - 1) {
				cursory += 1;
			}
		}
		if (ch == KEY_LEFT) {
			if (cursorx > 0) {
				cursorx -= 1;
			}
		}
		if (ch == KEY_RIGHT) {
			if (cursorx < col - 1) {
				cursorx += 1;
			}
		}
		if (ch == KEY_ENTER) {
			matrix[cursorx][cursory].invert();
		}
		if (ch == KEY_HOME) {
			edit = false;
			refresh();
			break;
		}
		if (ch == KEY_DL) {
			for (int xxx = 0; xxx < col - 1; ++xxx) {
				for (int yyy = 0; yyy < row - 1; ++yyy) {
					matrix[xxx][yyy].set(false);
				}
			}
		}
		if (ch == KEY_BACKSPACE) {
			for (int xxx = 0; xxx < col - 1; ++xxx) {
				for (int yyy = 0; yyy < row - 1; ++yyy) {
					if (matrix[xxx][yyy].off == '-') {
						matrix[xxx][yyy].off = ' ';
					} else {
						matrix[xxx][yyy].off = '-';
					}
				}
			}
		}
		if (ch == KEY_END) {
			break;
		}
	}
	while (not edit) {
		nodelay(stdscr,TRUE);
		clear();
		for (int y = 0; y < row - 1; ++y) {
			for (int x = 0; x < col - 1; ++x) {
				if (matrix[x][y].get()) {
					attron(COLOR_PAIR(1));
				} else {
					attron(COLOR_PAIR(3));
				}
				printw("%c",matrix[x][y].print());
				int sum = 0;
				for (int xx = std::max(0,x-1); xx <= std::min(col-2,x+1); ++xx) {
					for (int yy = std::max(0,y-1); yy <= std::min(row-2,y+1); ++yy) {
						if (not ((xx == x) && (yy == y))) {
							sum = sum + matrix[xx][yy];
						}
					}
				}
				if (matrix[x][y] == true) {
					if ((sum == 3) || (sum == 2)) {
						matrix[x][y].next(true);
					} else {
						matrix[x][y].next(false);
					}
				} else {
					if (sum == 3) {
						matrix[x][y].next(true);
					} else {
						matrix[x][y].next(false);
					}
				}
			}
			printw("\n");
		}
		attroff(COLOR_PAIR(1)); attroff(COLOR_PAIR(3));
		printw("Speed = %i",speed);
		printw("  Iteration = %i -PLAY-", iteration); 
		++iteration;
		refresh();
		usleep(abs(speed * 10000));
		for (int x = 0; x < col - 1; ++x) {
			for (int y = 0; y < row - 1; ++y) {
				matrix[x][y].update();
			}
		}
		if (ch = getch()) {
			if (ch == KEY_HOME) {
				edit = true;
				refresh();
				goto ed;
			}
			if (ch == KEY_END) {
				break;
			}
			if (ch == KEY_BACKSPACE) {
				for (int xxx = 0; xxx < col - 1; ++xxx) {
					for (int yyy = 0; yyy < row - 1; ++yyy) {
						if (matrix[xxx][yyy].off == '-') {
							matrix[xxx][yyy].off = ' ';
						} else {
							matrix[xxx][yyy].off = '-';
						}
					}
				}
			}
			if (ch == KEY_LEFT) {
				--speed;
				if (speed < 0) {
					speed = 1;
				}
			}
			if (ch == KEY_RIGHT) {
				++speed;
				if (speed > 13) {
					speed = 16;
				}
			}
		}
	}

	refresh();
	clear();
	nodelay(stdscr,FALSE);
	printw("Press any key to continue...");
	refresh();
	getch(); 
	endwin();
	return 0;	
}
