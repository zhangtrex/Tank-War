// Term--Fall 2020
// Chenyang Zhang

#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <chrono>
#include <glm/glm.hpp>

#include "cell.hpp"
#include "entity.hpp"
#include "block.hpp"
#include "tank.hpp"
#include "shell.hpp"
#include "enemy.hpp"

#include "../particle.hpp"

#define ENABLE_SOUND

#ifdef ENABLE_SOUND
#include "../sound.hpp"
#endif

class A5;

class World {
	std::vector<std::vector<Cell>> grid;
	std::pair<int, int> base_pos;
	std::vector<std::shared_ptr<Enemy>> enemy;
	std::vector<std::shared_ptr<EnemySpawner>> enemy_spawner;
	std::vector<std::shared_ptr<Shell>> shells;
	std::vector<std::shared_ptr<Block>> blocks;
	std::shared_ptr<Tank> player;
	std::chrono::steady_clock::time_point last_time;
	
	std::vector<std::shared_ptr<ParticleSystem>> explosions;
	int score;
	bool started;
	bool gameover;

#ifdef ENABLE_SOUND
	std::shared_ptr<Sound> sound_player;
#endif

public:
	
	friend A5;

	World();
	~World();
	void readMap();
	void printMap(); // testing purpose
	void setValue(int x, int y, int h); // set the occupy state of a cell
	int getValue(int x, int y); // ge the occupy state of a cell
	const std::vector<std::shared_ptr<Shell>> getShells();
	const std::vector<std::shared_ptr<Block>> getBlocks();
	const std::pair<int, int> getBasePos();
	int getDim();
	
	void reset(); // reset game
	void start(); // start a game
	void update(); // update game entities
	bool isGameover(); // if the game is over
	void checkGameover(); // update the gameover status
	void event(int code); // event for key pressing
	glm::vec3 getPlayerPos();
	glm::mat4 getRootTrans();
	

	void newShell(glm::vec3 pos, glm::vec3 dir, int s); // generate a new shell
	void newEnemy(int x, int y); // generate a new enemy
	
	void newExp(glm::vec3 pos); // generate a new explosion particle effect
#ifdef ENABLE_SOUND
	void playBGM(); // play background music
#endif
};

