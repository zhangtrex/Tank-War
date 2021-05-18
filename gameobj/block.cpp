// Term--Fall 2020
// Chenyang Zhang

#include <glm/glm.hpp>

#include "block.hpp"
#include "world.hpp"
using namespace glm;

Block::Block(int x, int y, bool d, World * world):
	Entity(2, vec3(float(x), 1.0f, float(y)), world),
	destroyable(d),
	opacity(1.0f)
{}

void Block::hit(int health) {
	if (destroyable) {
		hp -= health;
	}
}

bool Block::isDestroyable() {
	return destroyable;
}
