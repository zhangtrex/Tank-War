// Term--Fall 2020
// Chenyang Zhang

#pragma once

#include <glm/glm.hpp>
#include <memory>

class World;

class Block;
class Tank;
class Shell;


// Every game object is a Entity
class Entity {
protected:
	int hp; // hopefully it doesn't underflow (seems impossible)
	glm::vec3 position;
	World * w;

public:
	Entity() = default;
	Entity(int hp, glm::vec3 pos, World * World);
	~Entity() = default;
	int getHP() const;
	glm::vec3 getPos() const;
	virtual void hit(int health); // reduce the hp by health
	virtual void update(float t); // update the entity given time delta

	// return the transition matrix for rendering the entity, should get overrided
	virtual glm::mat4 getRootTrans();
};

