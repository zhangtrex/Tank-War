// Term--Fall 2020
// Chenyang Zhang

#pragma once

#include <glm/glm.hpp>

#include "entity.hpp"

class World;


// A class for a Block
class Block: public Entity {
	bool destroyable;
	float opacity;
public:
	Block(int x, int y, bool d, World * world);
	void hit(int health) override;
	bool isDestroyable();
};
