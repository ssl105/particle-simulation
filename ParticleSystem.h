#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "core.h"
#include "Particle.h"
#include "Ground.h"

#include <limits>
#include <time.h>

#define PI 3.14159265

using namespace std;

class ParticleSystem
{
private:
 
    glm::mat4 model;

    static const int MAX_PARTICLES = 10000;

    GLuint VAO, VBO;

    vector <Particle*> particles;
    vector <glm::vec3> position_data;

    glm::vec3 source, velocity, color;
    int particle_life;
    int creationRate;
    float size, restitution;
    float dragCoeff, friction;
    float mass;

    // variance variables
    glm::vec3 posVar;
    glm::vec3 velVar;
    int lifeVar;

public:
    ParticleSystem(glm::vec3 pos, glm::vec3 vel, int life, int rate, float s, float r, float m, float dc, float f);
    ~ParticleSystem();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update(float deltaTime, glm::vec3 gravity, glm::vec3 wind, float density);
    void updatePos();
    void updateBuffer();
    void checkGroundCollision(Ground* ground);

    // set mean constants
    void setCreationRate(int rate) { creationRate = rate; }
    void setLife(int life) { particle_life = life; }
    void setxPos(float x) { source.x = x; }
    void setyPos(float y) { source.y = y; }
    void setzPos(float z) { source.z = z; }
    void setxVel(float x) { velocity.x = x; }
    void setyVel(float y) { velocity.y = y; }
    void setzVel(float z) { velocity.z = z; }
    void setSize(float s) { size = s; }
    void setElasticity(float rest) { restitution = rest; }
    void setDrag(float dc) { dragCoeff = dc; }
    void setFriction(float f) { friction = f; }

    // set variance variables
    void setxPosVar(float x) { posVar.x = x; }
    void setyPosVar(float y) { posVar.y = y; }
    void setzPosVar(float z) { posVar.z = z; }
    void setxVelVar(float x) { velVar.x = x; }
    void setyVelVar(float y) { velVar.y = y; }
    void setzVelVar(float z) { velVar.z = z; }
    void setLifeVar(int l) { lifeVar = l; }
    
    // get mean constants
    int getCreationRate() { return creationRate; }
    int getLife() { return particle_life; }
    float getxPos() { return source.x; }
    float getyPos() { return source.y; }
    float getzPos() { return source.z; }
    float getxVel() { return velocity.x; }
    float getyVel() { return velocity.y; }
    float getzVel() { return velocity.z; }
    float getSize() { return size; }
    float getElasticity() { return restitution; }
    float getDrag() { return dragCoeff; }
    float getFriction() { return friction; }

    // get variance variables
    float getxPosVar() { return posVar.x; }
    float getyPosVar() { return posVar.y; }
    float getzPosVar() { return posVar.z; }
    float getxVelVar() { return velVar.x; }
    float getyVelVar() { return velVar.y; }
    float getzVelVar() { return velVar.z; }
    int getLifeVar() { return lifeVar; }
  
};

#endif
