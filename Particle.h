#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "core.h"

////////////////////////////////////////////////////////////////////////////////

class Particle
{
private:
	glm::vec3 position, velocity, force;
	float mass;
	int lifespan, life;


public:
	Particle(glm::vec3 pos, glm::vec3 vel, float m, int l);

	void intialize(glm::vec3 pos, glm::vec3 vel, int l);
	void applyForce(glm::vec3 f);
	void integrate(float deltaTime);

	glm::vec3 getPosition();
	glm::vec3 getVelocity();
	int getLife();
	int getLifespan();

	void setPosition(glm::vec3 pos);

	void addVelocity(glm::vec3 v);

};

////////////////////////////////////////////////////////////////////////////////

#endif
