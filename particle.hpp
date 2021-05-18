// Term--Fall 2020
// Chenyang Zhang

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <utility>
#include <iostream>
#include <vector>

class Particle {
	glm::vec3 position; // position of the particle
	glm::vec3 rotation; // rotation around x,y,z axis (not used)
	glm::vec3 direction; // direction of the movement of the particle
	float scale; // the scale of the particle
	float scale_factor;
	float finish_time; // when it finish
	float now_time; 
public:
	Particle(glm::vec3 pos, glm::vec3 rot, glm::vec3 dir, 
		float scale, float scale_f, float finish);

	glm::mat4 getTrans();
	bool isFinished(); // if it should finish
	void update(float t); // update the position of the particle
};

class ParticleSystem {
	std::vector<std::shared_ptr<Particle>> particles;
	glm::vec3 position;
	int num_particle = 20;
public:
	ParticleSystem(glm::vec3 pos);
	std::vector<glm::mat4> getTrans();
	void update(float t); // update particles
	bool isFinished(); // if the particle effect is finished
};


