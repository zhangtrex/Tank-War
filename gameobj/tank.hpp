// Term--Fall 2020
// Chenyang Zhang

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <utility>
#include <chrono>

#include "entity.hpp"

enum Direction {
	N=0, S=3, W=1, E=2
};

class World;
class Shell;

class Tank: public Entity {
protected:
	int true_x;
	int true_y;
	Direction direction;
	glm::vec3 dir_vec;
	float firing_speed;
	float last_fire;
	float move_speed;
	float last_move;
	int side;

	void updateDirVec();
	void setFiringSpeed(float f);
	void setMoveSpeed(float m);

public:
	Tank(int x, int y, int side, World * world, int health=2);
	bool canFire();
	void fire();
	bool getHit(float x, float y, int s);
	std::pair<int, int> getTruePos();
	void move(Direction d, bool dirOnly=false);
	void move();
	void update(float t) override;
	glm::mat4 getRootTrans() override;
};
