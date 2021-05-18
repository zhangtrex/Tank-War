// Term--Fall 2020
// Chenyang Zhang

#include <glm/glm.hpp>
#include <memory>
#include <iostream>

#include "shell.hpp"
#include "tank.hpp"
#include "block.hpp"
#include "world.hpp"

using namespace glm;
using namespace std;

Shell::Shell(vec3 pos, vec3 vel, int side, World * world):
	Entity(1, pos, world),
	velocity(vel),
	side(side)
{}


void Shell::update(float t) {
	position += velocity * t;
}

bool Shell::hitTank(shared_ptr<Tank> tank) {
	if (tank->getHit(position[0], position[2], side)) {
		return true;
	}
	return false;
}

bool Shell::hitShell(shared_ptr<Shell> shell) {
	return false;
}

bool Shell::hitBlock(shared_ptr<Block> block) {
	vec3 upper_left = block->getPos();
	vec3 lower_bottom = upper_left + vec3(1.0f, 0.0, 1.0f);
	if ((position[0] < lower_bottom[0]) && (position[0] > upper_left[0]) &&
		(position[2] < lower_bottom[2]) && (position[2] > upper_left[2])) {
		return true;
	}
	return false;
}

bool Shell::hitBase(float x, float y) {
	if (side == 0) {
		// return;
	}
	if ((position[0] < x+0.8f) && (position[0] > x+0.2f) &&
		(position[2] < y+0.8f) && (position[2] > y+0.2f)) {
		return true;
	}
	return false;
}

bool Shell::selfDestory() {
	return (position[0] < -10.0f) || (position[0] > 20.0f) || 
			(position[1] < -10.0f) || (position[1] > 20.0f);
}

int Shell::getSide() {
	return side;
}

