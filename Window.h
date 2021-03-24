#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Camera.h"
#include "Ground.h"
#include "ParticleSystem.h"

////////////////////////////////////////////////////////////////////////////////

class Window
{
public:
	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Objects to render
	static Ground* ground;
	static ParticleSystem* ps;

	// Ground constants
	static int size;
	static float elevation;

	// ps constants
	static float radius;
	static int life;
	static float mass;
	static float restitution;
	static float dragCoeff;
	static float frictionCoeff;
	static glm::vec3 pos;
	static glm::vec3 posVar;
	static glm::vec3 vel;
	static glm::vec3 velVar;
	static int lifeVar;
	static int rate;

	// gravity
	static glm::vec3 gravity;

	// wind speed
	static glm::vec3 windSpeed;
	static float density;

	// Animation speed;
	static float deltaTime;
	static int oversamples;

	// Shader Program 
	static GLuint shaderProgram;

	// Act as Constructors and desctructors 
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// for the Window
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// update and draw functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// helper to reset the camera
	static void resetCamera();

	// callbacks - for interaction
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double currX, double currY);

	// tweak bar call back functions
	static float getFloatVariable(string s);
	static void setFloatVariable(string s, const float f);
};

////////////////////////////////////////////////////////////////////////////////

#endif