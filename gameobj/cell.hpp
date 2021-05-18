// Term--Fall 2020
// Chenyang Zhang

#pragma once

#include <utility>
#include <memory>

#include "entity.hpp"

class Cell {
	int x_pos;
	int y_pos;
	bool is_occupy;
	std::shared_ptr<Entity> obj;
public:
	Cell();
	Cell(int x, int y);
	~Cell();
	std::pair<int, int> getPos();
	void setPos(const int x, const int y);
	void setOccupy(bool occ);
	bool isOccupy();
};
