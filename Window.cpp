////////////////////////////////////////
// Window.cpp
////////////////////////////////////////

#include "Window.h"

////////////////////////////////////////////////////////////////////////////////

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE 169 Starter";

// Objects to render
Ground* Window::ground;
ParticleSystem* Window::ps;

// Ground constants
int Window::size = 10;
float Window::elevation = -1.0f;

// ps variables
float Window::radius = 0.5f;
int Window::life = 1000;
float Window::mass = 1.0f;
float Window::restitution = 0.05f;
float Window::dragCoeff = 2;
float Window::frictionCoeff = 2;
glm::vec3 Window::pos = glm::vec3(0, 0, 0);
glm::vec3 Window::posVar = glm::vec3(0);
glm::vec3 Window::vel = glm::vec3(0);
glm::vec3 Window::velVar = glm::vec3(0);
int Window::lifeVar = 0;
int Window::rate = 1;

// gravity
glm::vec3 Window::gravity = glm::vec3(0, -9.8, 0);

// wind
glm::vec3 Window::windSpeed = glm::vec3(0, 0, 0);
float Window::density = 1.225;

// time variables
float Window::deltaTime = 0.001;
int Window::oversamples = 1;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;


////////////////////////////////////////////////////////////////////////////////

// Constructors and desctructors 
bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	ground = new Ground(size, elevation);
	ps = new ParticleSystem(pos, vel, life, rate, radius, restitution, mass, dragCoeff, frictionCoeff);

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete ps;
	delete ground;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

////////////////////////////////////////////////////////////////////////////////

// for the Window
GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// set up the camera
	Cam = new Camera();
	Cam->SetAspect(float(width) / float(height));

	// initialize the interaction variables
	LeftDown = RightDown = false;
	MouseX = MouseY = 0;

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	Cam->SetAspect(float(width) / float(height));

	TwWindowSize(width, height);
}

////////////////////////////////////////////////////////////////////////////////

// update and draw functions
void Window::idleCallback()
{
	// Perform any updates as necessary. 
	Cam->Update();

	// particle system animation
	ps->update(deltaTime, gravity, windSpeed, density);
	ps->checkGroundCollision(ground);
	ps->updateBuffer();

}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the object.
	ground->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
	ps->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();

}

////////////////////////////////////////////////////////////////////////////////

// helper to reset the camera
void Window::resetCamera() 
{
	Cam->Reset();
	Cam->SetAspect(float(Window::width) / float(Window::height));
}

////////////////////////////////////////////////////////////////////////////////

// callbacks - for Interaction 
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
		
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		case GLFW_KEY_R:
			resetCamera();
			break;

		default:
			break;
		}
	}
	

}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (!TwEventMouseButtonGLFW(button, action)) { // send event to tweak bar
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			LeftDown = (action == GLFW_PRESS);
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			RightDown = (action == GLFW_PRESS);
		}
	}
	
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

	MouseX = (int)currX;
	MouseY = (int)currY;

	if (!TwEventMousePosGLFW(MouseX, MouseY)) {	// send event to tweakbar
		// Move camera
		// NOTE: this should really be part of Camera::Update()
		if (LeftDown) {
			const float rate = 1.0f;
			Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
			Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
		}
		if (RightDown) {
			const float rate = 0.005f;
			float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
			Cam->SetDistance(dist);
		}
	}

}

////////////////////////////////////////////////////////////////////////////////

void Window::setFloatVariable(string s, const float f) {
	if (s == "xPos") {
		ps->setxPos(f);
	}
	else if (s == "yPos") {
		ps->setyPos(f);
	}
	else if (s == "zPos") {
		ps->setzPos(f);
	}
	else if (s == "xVel") {
		ps->setxVel(f);
	}
	else if (s == "yVel") {
		ps->setyVel(f);
	}
	else if (s == "zVel") {
		ps->setzVel(f);
	}
	else if (s == "rate") {
		ps->setCreationRate((int)f);
	}
	else if (s == "life") {
		ps->setLife((int)f);
	}
	else if (s == "gravity") {
		gravity.y = f;
	}
	else if (s == "density") {
		density = f;
	}
	else if (s == "drag") {
		ps->setDrag(f);
	}
	else if (s == "radius") {
		ps->setSize(f);
	}
	else if (s == "elasticity") {
		ps->setElasticity(f);
	}
	else if (s == "friction") {
		ps->setFriction(f);
	}
	else if (s == "xPosVar") {
		ps->setxPosVar(f);
	}
	else if (s == "yPosVar") {
		ps->setyPosVar(f);
	}
	else if (s == "zPosVar") {
		ps->setzPosVar(f);
	}
	else if (s == "xVelVar") {
		ps->setxVelVar(f);
	}
	else if (s == "yVelVar") {
		ps->setyVelVar(f);
	}
	else if (s == "zVelVar") {
		ps->setzVelVar(f);
	}
	else if (s == "lifeVar") {
		ps->setLifeVar((int)f);
	}
	else {
		
	}
}

////////////////////////////////////////////////////////////////////////////////

float Window::getFloatVariable(string s) {
	if (s == "xPos") {
		return ps->getxPos();
	}
	else if (s == "yPos") {
		return ps->getyPos();
	}
	else if (s == "zPos") {
		return ps->getzPos();
	}
	else if (s == "xVel") {
		return ps->getxVel();
	}
	else if (s == "yVel") {
		return ps->getyVel();
	}
	else if (s == "zVel") {
		return ps->getzVel();
	}
	else if (s == "rate") {
		return (float)ps->getCreationRate();
	}
	else if (s == "life") {
		return (float)ps->getLife();
	}
	else if (s == "gravity") {
		return gravity.y;
	}
	else if (s == "density") {
		return density;
	}
	else if (s == "drag") {
		return ps->getDrag();
	}
	else if (s == "radius") {
		return ps->getSize();
	}
	else if (s == "elasticity") {
		return ps->getElasticity();
	}
	else if (s == "friction") {
		return ps->getFriction();
	}
	else if (s == "xPosVar") {
		return ps->getxPosVar();
	}
	else if (s == "yPosVar") {
		return ps->getyPosVar();
	}
	else if (s == "zPosVar") {
		return ps->getzPosVar();
	}
	else if (s == "xVelVar") {
		return ps->getxVelVar();
	}
	else if (s == "yVelVar") {
		return ps->getyVelVar();
	}
	else if (s == "zVelVar") {
		return ps->getzVelVar();
	}
	else if (s == "lifeVar") {
		return (float)ps->getLifeVar();
	}
	else {
		return 0;
	}
}