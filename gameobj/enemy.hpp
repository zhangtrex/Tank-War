// Term--Fall 2020
// Chenyang Zhang

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <utility>
#include <iostream>

#include "tank.hpp"
#include "entity.hpp"

class World;

class Enemy: public Tank {
	int base_x;
	int base_y;

	// select a random direction for a enemy
	void randDir();
	
	// stop moving action for some time
	void sleep();

public:
	Enemy(int x, int y, World * world, int b_x, int b_y);
	~Enemy();
	void enemyUpdate(float t);
};

// Defined to spawn enemy based on some rules
class EnemySpawner: public Entity {
	int x;
	int y;
	float last_spawn;
	float spawn_time;
	int side;

public:
	EnemySpawner(int x, int y, float spawn_time, World * world);
	void update(float t) override;
	void spawn(); // try to spawn an enemy
	void sleep(float t);
};
