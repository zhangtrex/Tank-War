// Term--Fall 2020
// Chenyang Zhang

#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "entity.hpp"

class World;
class Tank;
class Block;

// shell class
class Shell: public Entity {
	glm::vec3 velocity;
	int side; // 1 for Enemy Tank Shell, 0 for Player's Tank Shell
	
public:
	Shell(glm::vec3 pos, glm::vec3 vel, int side, World * world);
	void update(float t) override;
	bool hitTank(std::shared_ptr<Tank> tank); // if it hit a tank
	bool hitShell(std::shared_ptr<Shell> shell); // if it hit a shell, NOTE: not used.
	bool hitBlock(std::shared_ptr<Block> block); // if it hit a block
	bool hitBase(float x, float y); // if it hit the base (puppet)
	bool selfDestory(); // not used
	int getSide();
};
