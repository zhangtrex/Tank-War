// Term--Fall 2020
// Chenyang Zhang

#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <chrono>
#include <glm/glm.hpp>
#include <ctime>

#include "world.hpp"
#include "cell.hpp"
#include "entity.hpp"
#include "block.hpp"
#include "tank.hpp"
#include "shell.hpp"
#include "../sound.hpp"
#include "../particle.hpp"

using namespace std;
using namespace glm;

static const int MAX_ENEMY = 6;

string default_filename = "default_map.txt";

World::World() {
#ifdef ENABLE_SOUND
	sound_player = make_shared<Sound>();
	sound_player->init();
	sound_player->setVolume(0.5f);
	playBGM();
#endif
	score = 0;
	started = false;
	gameover = false;
}

World::~World() {}

void World::readMap() {
	string line;
	ifstream file (default_filename);
	if (file.is_open()) {
		int i = 0;
		while (getline(file, line)) {
			if (line.length() > 0) {
				vector<Cell> cells;
				for (int j = 0; j < line.length(); j++) {
					Cell c(j, i);
					if (line[j] == 'w') {
						// undestroyable walls
						c.setOccupy(true);
						blocks.push_back(make_shared<Block>(j, i, false, this));
					}
					else if (line[j] == 'b') {
						// base
						base_pos = make_pair(j, i);
						c.setOccupy(true);
					}
					else if (line[j] == 's') {
						// enemy spawn points
						enemy_spawner.push_back(make_shared<EnemySpawner>(j, i, 5, this));
					}
					else if (line[j] == 'd') {
						// destroyable walls
						c.setOccupy(true);
						blocks.push_back(make_shared<Block>(j, i, true, this));
					}
					else {
						// i dont know
					}
					cells.push_back(c);
				}
				grid.push_back(cells);
			}
			else {
				break;
			}
			i ++;
		}
		file.close();
	}
	// for (int i = 0; i < grid.size(); i++) {
	// 	vector<Cell> this_line = grid.at(i);
	// 	for (int j = 0; j < this_line.size(); j++) {
	// 		this_line.at(j).setPos(i, j);
	// 	}
	// }
	int x_pos = base_pos.first+1;
	int y_pos = getDim()-2;
	for (int i = x_pos; i < 11; i++) {
		if (grid.at(y_pos).at(i).isOccupy() == 0) {
			x_pos = i;
			break;
		}
	}
	player = make_shared<Tank>(x_pos, y_pos, 0 , this, 4);
	last_time = chrono::steady_clock::now();
}

void World::printMap() {
	for (int i = 0; i < grid.size(); i++) {
		vector<Cell> this_line = grid.at(i);
		for (int j = 0; j < this_line.size(); j++) {
			if (this_line.at(j).isOccupy()) {
				cout << "X";
			}
			else {
				cout << " ";
			}
		}
		cout << endl;
	}
}

void World::setValue(int x, int y, int h) {
	grid.at(y).at(x).setOccupy(h);
}

int World::getValue(int x, int y) {
	return grid.at(y).at(x).isOccupy();
}

const vector<shared_ptr<Shell>> World::getShells() {
	return shells;
}

const vector<shared_ptr<Block>> World::getBlocks() {
	return blocks;
}

const std::pair<int, int> World::getBasePos() {
	return base_pos;
}

void World::reset() {

	shells.clear();
	enemy.clear();
	explosions.clear();
	blocks.clear();
	grid.clear(); // grid has to be in the end
	started = false;
	gameover = false;
	score = 0;

}

void World::start() {
	
	started = true;
}

bool World::isGameover() {
	return gameover;
}

void World::checkGameover() {
	if (player->getHP() <= 0) {
		gameover = true;
	}
}

void World::update() {
	if (!started) {
		return;
	}
	auto now_time = chrono::steady_clock::now();
	float duration = chrono::duration_cast<chrono::duration<float>>(now_time-last_time).count();
	
	for (auto s: shells) {
		s->update(duration);
	}

	player->update(duration);

	if (enemy.size() <= MAX_ENEMY) {
		for (auto es: enemy_spawner) {
			es->update(duration);
		}
	}

	for (auto e: enemy) {
		e->enemyUpdate(duration);
	}

	for (auto s: shells) {
		for (auto b: blocks) {
			if (s->hitBlock(b)) {
				// TODO: change this to adapt different damage.
				s->hit(1);
				b->hit(1); 
				break;
			}
		}
		for (auto e: enemy) {
			if (s->hitTank(e)) {
				s->hit(1);
				e->hit(1);
			}
		}
		if (s->hitTank(player) && !gameover) {
			s->hit(1);
			player->hit(1);
			checkGameover();
			if (gameover) {
				vec3 pos = player->getPos();
				setValue(pos[0], pos[2], 0);
				pos += vec3(0.0f,0.5f,0.0f);
				pos -= vec3(float(getDim())/2.0f+0.5f, 0.0f, float(getDim())/2.0f+0.5f);
				newExp(pos);
#ifdef ENABLE_SOUND
				sound_player->playExplosion();
#endif
			}
		}
		if (s->hitBase(base_pos.first, base_pos.second) && !gameover) {
			gameover = true;
			vec3 pos = vec3(base_pos.first, 0, base_pos.second);
			setValue(pos[0], pos[2], 0);
			pos += vec3(0.0f,0.5f,0.0f);
			pos -= vec3(float(getDim())/2.0f, 0.0f, float(getDim())/2.0f);
			newExp(pos);
#ifdef ENABLE_SOUND
			sound_player->playExplosion();
#endif
		}
	}



	for (auto ps: explosions) {
		ps->update(duration);
	}

	// TODO: improve performance by adding a flag of any deletion.

	for (auto it = shells.begin(); it != shells.end();) {
		if ((*it)->getHP() <= 0) {
			shells.erase(it);
			continue;
		}
		it++;
	}

	for (auto it = blocks.begin(); it != blocks.end();) {
		if ((*it)->getHP() <= 0) {
			vec3 b_pos = (*it)->getPos();
			int x = b_pos[0];
			int z = b_pos[2];
			setValue(x,z,0);
			blocks.erase(it);
			continue;
		}
		it++;
	}

	for (auto it = explosions.begin(); it != explosions.end();) {

		if ((*it)->isFinished()) {
			explosions.erase(it);
			continue;
		}
		it++;
	}

	for (auto it = enemy.begin(); it != enemy.end();) {
		if ((*it)->getHP() <= 0) {
#ifdef ENABLE_SOUND
			sound_player->playExplosion();
#endif
			vec3 e_pos = (*it)->getPos();
			e_pos += vec3(0.0f,0.5f,0.0f);
			e_pos -= vec3(float(getDim())/2.0f+0.5f, 0.0f, float(getDim())/2.0f+0.5f);
			newExp(e_pos);
			enemy.erase(it);
			score += 100;
			continue;
		}
		it++;
	}


	last_time = now_time;
}

void World::event(int code) {
	if (!started) {
		return;
	}
	if (gameover) {
		return;
	}
	switch (code) {
		case 0: {
			player->move(Direction::N);
			break;
		}
		case 1: {
			player->move(Direction::S);
			break;
		}
		case 2: {
			player->move(Direction::W);
			break;
		}
		case 3: {
			player->move(Direction::E);
			break;
		}
		case 4: {
			player->fire();
			break;
		}
		default: {
			break;
		}
	}
}

mat4 World::getRootTrans() {
	return player->getRootTrans();
}

vec3 World::getPlayerPos() {
	return player->getPos();
}

void World::newShell(vec3 pos, vec3 dir, int s) {
	auto new_shell = make_shared<Shell>(pos, dir, s, this);
	shells.push_back(new_shell);
}

void World::newEnemy(int x, int y) {
	shared_ptr<Enemy> new_enemy = make_shared<Enemy>(x, y, this, base_pos.first, base_pos.second);
	enemy.push_back(new_enemy);
}

int World::getDim() {
	return grid.size();
}

#ifdef ENABLE_SOUND
void World::playBGM() {
	sound_player->play();
}
#endif

void World::newExp(vec3 pos) {
	shared_ptr<ParticleSystem> new_ps = make_shared<ParticleSystem>(pos);
	explosions.push_back(new_ps);
} 


