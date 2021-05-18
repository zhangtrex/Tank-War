// Term--Fall 2020
// Chenyang Zhang

#include <glm/glm.hpp>
#include <memory>
#include <utility>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "cs488-framework/MathUtils.hpp"

#include "tank.hpp"
#include "shell.hpp"
#include "world.hpp"

using namespace std;
using namespace glm;

Tank::Tank(int x, int y, int side, World * world, int health):
	Entity(health, vec3(float(x)+0.5f, 0.0f, float(y)+0.5f), world),
	true_x(x),
	true_y(y),
	direction(Direction::N),
	dir_vec(vec3(0.0f,0.0f,-1.0f)),
	firing_speed(1.0f),
	last_fire(0.0f),
	move_speed(0.5f),
	last_move(0.0f),
	side(side)
{
	w->setValue(x,y,true);
}


void Tank::updateDirVec() {
	switch (direction) {
		case Direction::N: {
			dir_vec = vec3(0.0f, 0.0f, -1.0f);
			break;
		}
		case Direction::S: {
			dir_vec = vec3(0.0f, 0.0f, 1.0f);
			break;
		}
		case Direction::W: {
			dir_vec = vec3(-1.0f, 0.0f, 0.0f);
			break;
		}
		case Direction::E: {
			dir_vec = vec3(1.0f, 0.0f, 0.0f);
			break;
		}
	}
}

void Tank::setFiringSpeed(float f) {
	firing_speed = f;
}

void Tank::setMoveSpeed(float m) {
	move_speed = m;
}


pair<int, int> Tank::getTruePos() {
	return pair<int,int>(true_x, true_y);
}

bool Tank::canFire() {
	return (last_fire >= firing_speed);
}

void Tank::fire() {
	if (last_fire < firing_speed) {
		return;
	}
	shared_ptr<Shell> s;
	switch (direction) {
		case Direction::W: {
			w->newShell(position+vec3(-0.25f, 0.25f, 0.0f), vec3(-4.0f, 0.0f, 0.0f), side);
			break;
		}
		case Direction::E: {
			w->newShell(position+vec3(0.25f, 0.25f, 0.0f), vec3(4.0f, 0.0f, 0.0f), side);
			break;
		}
		case Direction::N: {
			w->newShell(position+vec3(0.0f, 0.25f, -0.25f), vec3(0.0f, 0.0f, -4.0f), side);
			break;
		}
		case Direction::S: {
			w->newShell(position+vec3(0.0f, 0.25f, 0.25f), vec3(0.0f, 0.0f, 4.0f), side);
			break;
		}
	}
	last_fire = 0.0f;
}

bool Tank::getHit(float x, float y, int s) {
	if (s == side) return false;
	if (abs(x-position[0]) < 0.5 && abs(y-position[2]) < 0.5) {
		return true;
	}
	return false;
}

void Tank::move(Direction d, bool dirOnly) {
	if (direction != d) {
		direction = d;
		updateDirVec();
		return;
	}
	if (dirOnly) {
		return;
	}
	if (last_move < move_speed) {
		return;
	}

	vec3 new_pos = position + dir_vec;
	if (w->getValue(new_pos[0], new_pos[2])) {
		return;
	}

	switch (direction) {
		case Direction::N: {
			if (w->getValue(true_x, true_y-1)) return;
			w->setValue(true_x, true_y, false);
			w->setValue(true_x, true_y-1, true);
			dir_vec = vec3(0.0f, 0.0f, -1.0f);
			position += dir_vec;
			true_y += -1;
			
			break;
		}
		case Direction::S: {
			if (w->getValue(true_x, true_y+1)) return;
			w->setValue(true_x, true_y, false);
			w->setValue(true_x, true_y+1, true);
			dir_vec = vec3(0.0f, 0.0f, 1.0f);
			position += dir_vec;
			true_y += 1;
			
			break;
		}
		case Direction::W: {
			if (w->getValue(true_x-1, true_y)) return;
			w->setValue(true_x, true_y, false);
			w->setValue(true_x-1, true_y, true);
			dir_vec = vec3(-1.0f, 0.0f, 0.0f);
			position += dir_vec;
			true_x += -1;
			
			break;
		}
		case Direction::E: {
			if (w->getValue(true_x+1, true_y)) return;
			w->setValue(true_x, true_y, false);
			w->setValue(true_x+1, true_y, true);
			dir_vec = vec3(1.0f, 0.0f, 0.0f);
			position += dir_vec;
			true_x += 1;
			
			break;
		}
	}
	last_move = 0.0f;
	// w->printMap();

	// cout << true_x << " " << true_y << endl;
}

void Tank::move() {
	move(direction);
}


void Tank::update(float t) {
	last_fire += t;
	last_move += t;

}

mat4 Tank::getRootTrans() {
	mat4 trans = mat4(1.0f);
	switch (direction) {
		case Direction::N: {
			trans = rotate(trans, degreesToRadians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
			break;
		}
		case Direction::E: {
			trans = rotate(trans, degreesToRadians(180.0f), vec3(0.0f, 1.0f, 0.0f));
			break;
		}
		case Direction::S: {
			trans = rotate(trans, degreesToRadians(90.0f), vec3(0.0f, 1.0f, 0.0f));
			break;
		}
	}
	return trans;
}


