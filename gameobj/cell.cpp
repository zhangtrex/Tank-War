// Term--Fall 2020
// Chenyang Zhang

#include <utility>

#include "cell.hpp"
#include "entity.hpp"

using namespace std;

Cell::Cell(): x_pos(0), y_pos(0), is_occupy(false), obj(nullptr) {}

Cell::Cell(int x, int y): x_pos(x), y_pos(y), is_occupy(false), obj(nullptr) {}

Cell::~Cell() {}

pair<int, int> Cell::getPos() {
	return pair<int, int>(x_pos, y_pos);
}

void Cell::setPos(const int x, const int y) {
	x_pos = x;
	y_pos = y;
}

void Cell::setOccupy(bool occ) {
	is_occupy = occ;
}

bool Cell::isOccupy() {
	return is_occupy;
}



