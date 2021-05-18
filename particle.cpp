// Term--Fall 2020
// Chenyang Zhang

#include <glm/glm.hpp>

#include <memory>
#include <utility>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <ctime>

#include "particle.hpp"

using namespace std;
using namespace glm;

Particle::Particle(vec3 pos, vec3 rot, vec3 dir, float scale, float scale_f, float finish):
	position(pos),
	rotation(rot),
	direction(dir),
	scale(scale),
	scale_factor(scale_f),
	finish_time(finish),
	now_time(0.0f)
{}

mat4 Particle::getTrans() {
	mat4 trans = mat4(1.0f);
	trans = translate(trans, position);
	trans = glm::scale(trans, scale * vec3(1.0f));
	trans = rotate(trans, rotation[0], vec3(1.0f,0.0f,0.0f));
	trans = rotate(trans, rotation[1], vec3(0.0f,1.0f,0.0f));
	trans = rotate(trans, rotation[2], vec3(0.0f,0.0f,1.0f));
	return trans;
}

bool Particle::isFinished() {
	return (finish_time < now_time);
}

void Particle::update(float t) {
	now_time += t;
	position += direction * t;
	vec3 random_dir = vec3(0.0f);
	default_random_engine generator;
	generator.seed(time(NULL));
	for (int i = 0; i < 3; i++) {
		random_dir[i] = normal_distribution<double>(0.0, 1.0)(generator);
	}
	random_dir = normalize(random_dir);
	random_dir = random_dir * t * 0.3f * length(direction);
	position += random_dir;
	scale -= t * (1.0f-scale_factor) * scale;
}


ParticleSystem::ParticleSystem(vec3 pos):
	position(pos) 
{
	// generate particles

	default_random_engine generator;
	generator.seed(time(NULL));
	for (int i = 0; i < num_particle; i++) {
		vec3 p_pos = pos;
		vec3 p_dir = vec3(0.0f);
		for (int i = 0; i < 3; i++) {
			p_dir[i] = normal_distribution<double>(0.0, 1.0)(generator);
		}
		p_dir = normalize(p_dir);
		p_pos += 0.05f * p_dir;
		float dir_factor = uniform_real_distribution<double>(0.3,1.0)(generator);
		p_dir *= dir_factor;

		vec3 p_rot = vec3(0.0f);

		float scale = uniform_real_distribution<double>(0.1,0.3)(generator);
		float stay = uniform_real_distribution<double>(0.8,1.2)(generator);
		shared_ptr<Particle> new_p = make_shared<Particle>(p_pos, p_rot, p_dir, scale, 0.8f, stay);
		particles.push_back(new_p);
	}
}

vector<mat4> ParticleSystem::getTrans() {
	vector<mat4> p_trans;
	for (auto p: particles) {
		mat4 new_trans = p->getTrans();
		p_trans.push_back(new_trans);
	}
	return p_trans;
}

void ParticleSystem::update(float t) {
	for (auto p: particles) {
		p->update(t);
	}
	for (auto it = particles.begin(); it != particles.end();) {
		if ((*it)->isFinished()) {
			particles.erase(it);
			continue;
		}
		it++;
	}
}

bool ParticleSystem::isFinished() {
	return (particles.size() <= 0);
}


