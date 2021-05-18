// Term--Fall 2020
// Chenyang Zhang

#include <glm/glm.hpp>
#include <memory>
#include <utility>
#include <iostream>
#include <stdlib.h>

#include "enemy.hpp"
#include "world.hpp"
#include "tank.hpp"

using namespace std;
using namespace glm;

Enemy::Enemy(int x, int y, World * world, int b_x, int b_y):
	Tank(x, y, 1, world),
	base_x(b_x),
	base_y(b_y)
{
	setMoveSpeed(1.22f);
	setFiringSpeed(2.0f);
}

Enemy::~Enemy() {
	w->setValue(position[0], position[2], 0);
}

void Enemy::randDir() {
	vec3 new_pos = dir_vec + position;
	if (w->getValue(new_pos[0], new_pos[2])) {
		int r = rand() % 4;
		Direction d = static_cast<Direction>(r);
		move(d);
		sleep();
	}
	else {
		int r = rand() % 6;
		if (r > 3) {
			r = 3;
		}
		Direction d = static_cast<Direction>(r);
		move(d);
		sleep();
	}
}

void Enemy::enemyUpdate(float t) {
	update(t);

	int r = rand() % 3;
	if (!r) {
		fire();
	}
	if (last_move < move_speed) return;
	r = rand() % 5;
	if (!r) {
		randDir();
	}
	else {
		vec3 new_pos = dir_vec + position;

		if (w->getValue(new_pos[0], new_pos[2])) {
			randDir();
		}
		else {

			move();
		}
	}

}

void Enemy::sleep() {
	last_move = 0.0f;
}


EnemySpawner::EnemySpawner(int x, int y, float spawn_time, World * world):
	Entity(1, vec3(float(x), 0.0f, float(y)), world),
	x(x),
	y(y),
	last_spawn(0),
	spawn_time(spawn_time)
{}

void EnemySpawner::update(float t) {
	last_spawn += t;
	if (last_spawn > spawn_time) {
		spawn();
	}
}

void EnemySpawner::spawn() {
	if (w->getValue(x,y)) {
		sleep(2);
		return;
	}
	int r = rand()%3;

	if (r) {
		sleep(2);
	}
	else {
		w->newEnemy(x,y);
		last_spawn = 0.0f;
	}
}

void EnemySpawner::sleep(float t) {
	if (t > spawn_time) {
		last_spawn = 0.0f;
	}
	else if (t <= 0.0f) {
		return;
	}
	else {
		last_spawn = spawn_time - t;
	}
}


