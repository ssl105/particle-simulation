#include "ParticleSystem.h"

//particle system functions
ParticleSystem::ParticleSystem(glm::vec3 pos, glm::vec3 vel, int life, int rate,  float s, float r, float m, float dc, float f) {
	srand((unsigned)time(NULL)); // rng

	//intialize mean variables
	color = glm::vec3(1, 0, 0);
	model = glm::mat4(1);
	source = pos;
	particle_life = life;
	size = s;
	restitution = r;
	velocity = vel;
	mass = m;
	creationRate = rate;
	dragCoeff = dc;
	friction = f;

	// intialize variance variables
	posVar = glm::vec3(0, 0, 0);
	velVar = glm::vec3(1);
	lifeVar = 30;

	glm::vec3 deltaPos(0);
	glm::vec3 deltaVel(0);
	int deltaLife = 0;
	int clife;
	for (int i = 0; i < creationRate; i++) {
		// random position shift
		if (posVar.x != 0) {
			deltaPos.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / posVar.x) - posVar.x / 2;
		}
		if (posVar.y != 0) {
			deltaPos.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / posVar.y) - posVar.y / 2;
		}
		if (posVar.z != 0) {
			deltaPos.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / posVar.z) - posVar.z / 2;
		}
		
		
		// random velocity shift
		if (velVar.x != 0) {
			deltaVel.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / velVar.x) - velVar.x / 2;
		}
		if (velVar.y != 0) {
			deltaVel.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / velVar.y) - velVar.y / 2;
		}
		if (velVar.z != 0) {
			deltaVel.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / velVar.z) - velVar.z / 2;
		}

		// random life
		if (lifeVar != 1) {
			deltaLife = rand() % lifeVar - (lifeVar / 2);
		}
		clife = glm::clamp(deltaLife + particle_life, 1, INT_MAX);

		particles.push_back(new Particle(source + deltaPos, velocity + deltaVel, mass, clife));
	}

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_PARTICLES, position_data.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

ParticleSystem::~ParticleSystem()
{
	// Delete particles
	for (int i = 0; i < particles.size(); i++) {
		delete particles[i];
	}

	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void ParticleSystem::draw(const glm::mat4& viewProjMtx, GLuint shader) {


	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
	glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Set point size
	glPointSize(2*size);

	// Draw the points 
	glDrawArrays(GL_POINTS, 0, position_data.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);


}

void ParticleSystem::update(float deltaTime, glm::vec3 gravity, glm::vec3 wind, float density) {

	// delete particles
	vector<Particle*>::iterator it = particles.begin();
	while (it != particles.end()) {
		if ((*it)->getLife() >= (*it)->getLifespan()) {
			delete (*it);
			it = particles.erase(it);
		}
		else {
			it++;
		}
	}
	
	// apply gravity
	gravity = mass * gravity;
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->applyForce(gravity);;
	}

	// apply aerodynamics
	for (int i = 0; i < particles.size(); i++) {
		glm::vec3 vp = particles[i]->getVelocity();
		glm::vec3 v = vp - wind;
		float vlength = glm::length(v);
		
		if (vlength == 0) {
			continue;
		}

		glm::vec3 e = -glm::normalize(v);
		float a = PI * size * size;
		
		glm::vec3 fdrag = (0.5f) * density * vlength * vlength * dragCoeff * a * e;
		particles[i]->applyForce(fdrag);

	}

	// integrate forces
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->integrate(deltaTime);
	}

	// create particles
	glm::vec3 deltaPos(0);
	glm::vec3 deltaVel(0);
	int deltaLife = 0;
	int life;
	for (int i = 0; i < creationRate; i++) {
		if (particles.size() >= MAX_PARTICLES) {
			break;
		}

		// random position shift
		if (posVar.x != 0) {
			deltaPos.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / posVar.x) - posVar.x / 2;
		}
		if (posVar.y != 0) {
			deltaPos.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / posVar.y) - posVar.y / 2;
		}
		if (posVar.z != 0) {
			deltaPos.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / posVar.z) - posVar.z / 2;
		}


		// random velocity shift
		if (velVar.x != 0) {
			deltaVel.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / velVar.x) - velVar.x / 2;
		}
		if (velVar.y != 0) {
			deltaVel.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / velVar.y) - velVar.y / 2;
		}
		if (velVar.z != 0) {
			deltaVel.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / velVar.z) - velVar.z / 2;
		}

		// random life
		if (lifeVar != 1) {
			deltaLife = rand() % lifeVar - (lifeVar / 2);
		}
		life = glm::clamp(deltaLife + particle_life, 1, INT_MAX);

		particles.push_back(new Particle(source + deltaPos, velocity + deltaVel, mass, life));
	}

}

void ParticleSystem::updatePos() {
	// reset positions
	position_data.clear();
	for (int i = 0; i < particles.size(); i++) {
		position_data.push_back(particles[i]->getPosition());
	}

}

void ParticleSystem::updateBuffer() {
	updatePos();

	// reset vbo
	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * position_data.size(), position_data.data());
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleSystem::checkGroundCollision(Ground* ground) {
	float elevation = ground->getElevation();
	glm::vec3 gNorm = ground->getNormal();
	bool collision = false;

	for (int i = 0; i < particles.size(); i++) {
		glm::vec3 pos = particles[i]->getPosition();

		if (pos.y < elevation) {
			if (!collision) {
				collision = true;
			}

			// handle collison
			glm::vec3 v = particles[i]->getVelocity();
			glm::vec3 vNorm = glm::dot(v, gNorm) * gNorm;
			glm::vec3 vTan = v - vNorm;
			glm::vec3 impulse = -(1 + restitution) * mass * vNorm;
			glm::vec3 fimpulse = friction * glm::length(impulse) * -vTan;
			impulse = impulse + fimpulse;
			glm::vec3 deltav = impulse / mass;

			particles[i]->addVelocity(deltav);	// apply impulse to velociy

			pos.y = elevation + 0.001;

			particles[i]->setPosition(pos);	// push particle above levation
		}
	}
}