// Term--Fall 2020
// Chenyang Zhang

#include <glm/glm.hpp>

#include "entity.hpp"
#include "world.hpp"

using namespace glm;

Entity::Entity(int hp, vec3 pos, World * world):
	hp(hp),
	position(pos),
	w(world)
{}

int Entity::getHP() const {
	return hp;
}

vec3 Entity::getPos() const {
	return position;
}

void Entity::hit(int health) {
	hp -= health;
}

void Entity::update(float t) {
}

mat4 Entity::getRootTrans() {
	return mat4(1.0f);
}


