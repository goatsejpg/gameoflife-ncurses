#ifndef CELL_H
#define CELL_H

#include <iostream>

class Cell {
	friend std::ostream& operator<<(const std::ostream&, const Cell&);
	public:
		Cell();
		char on = '#';
		char off = '.';
		void set(bool in) {_state = in;}
		bool get() const {return _state;}
		void invert() {_state = not _state;}
		void next(bool in) {_nextstate = in;}
		void update() {_state = _nextstate;}
		char print() {
			return _state ? on : off;
		}
	private:
		bool _state = false;
		bool _nextstate = false;
};

Cell::Cell(void) {}

inline int operator+(const Cell& a, const Cell& b) {
	return (a.get() ? 1 : 0) + (b.get() ? 1 : 0);
}

inline int operator+(const int& num, const Cell& cell) {
	return num + (cell.get() ? 1 : 0);
}

inline bool operator==(const Cell& cell, const bool& check) {
	return (cell.get() == check);
}

std::ostream& operator<<(std::ostream& out, const Cell& cell) {
	out << cell.get() ? cell.on : cell.off;
	return out;
}

#endif // CELL_H
