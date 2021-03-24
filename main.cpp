#include "main.h"


////////////////////////////////////////////////////////////////////////////////

void error_callback(int error, const char* description)
{
	// Print error.
	std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window)
{
	// Set the error callback.
	glfwSetErrorCallback(error_callback);
	// Set the window resize callback.
	glfwSetWindowSizeCallback(window, Window::resizeCallback);
	
	// Set the key callback.
	glfwSetKeyCallback(window, Window::keyCallback);

	// Set the mouse and cursor callbacks
	glfwSetMouseButtonCallback(window, Window::mouse_callback);
	glfwSetCursorPosCallback(window, Window::cursor_callback);
}

void setup_opengl_settings()
{
	// Enable depth buffering.
	glEnable(GL_DEPTH_TEST);
	// Related to shaders and z value comparisons for the depth buffer.
	glDepthFunc(GL_LEQUAL);
	// Set polygon drawing mode to fill front and back of each polygon.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Set clear color to black.
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions()
{
	// Get info of GPU and supported OpenGL version.
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) 
		<< std::endl;

	//If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::cout << "Supported GLSL version is: " << 
		glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}



////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	TwBar* bar;         // Pointer to a tweak bar

	// Create the GLFW window.
	GLFWwindow* window = Window::createWindow(800, 600);
	if (!window) exit(EXIT_FAILURE);

	// Initialize AntTweakBar
	TwInit(TW_OPENGL, NULL);

	// Create a tweak bar
	bar = TwNewBar("Adjustable Variables");
	TwDefine(" GLOBAL help='Change variables to interact with the particle system' ");

	// mean variables
	TwAddVarCB(bar, "creation rate", TW_TYPE_INT32, setRateCB, getRateCB , nullptr,
		" group='Mean' step=1 min=0 help='Number of particles spawned per frame' ");
	TwAddVarCB(bar, "xPos", TW_TYPE_FLOAT, setxPosCB, getxPosCB , nullptr,
		" group='Mean' step=0.01 help='X Position of particle source' ");
	TwAddVarCB(bar, "yPos", TW_TYPE_FLOAT, setyPosCB, getyPosCB , nullptr,
		" group='Mean' step=0.01 help='Y Position of particle source' ");
	TwAddVarCB(bar, "zPos", TW_TYPE_FLOAT, setzPosCB, getzPosCB , nullptr,
		" group='Mean' step=0.01 help='Z Position of particle source' ");
	TwAddVarCB(bar, "xVel", TW_TYPE_FLOAT, setxVelCB, getxVelCB, nullptr,
		" group='Mean' step=0.01 help='X velocity of particle source' ");
	TwAddVarCB(bar, "yVel", TW_TYPE_FLOAT, setyVelCB, getyVelCB, nullptr,
		" group='Mean' step=0.01 help='Y velocity of particle source' ");
	TwAddVarCB(bar, "zVel", TW_TYPE_FLOAT, setzVelCB, getzVelCB, nullptr,
		" group='Mean' step=0.01 help='Z velocity of particle source' ");
	TwAddVarCB(bar, "life", TW_TYPE_INT32, setLifeCB, getLifeCB, nullptr,
		" group='Mean' step=1 min=0 help='Lifespan of a particle' ");
	TwAddVarCB(bar, "gravity", TW_TYPE_FLOAT, setGravityCB, getGravityCB, nullptr,
		" group='Mean' step=0.01 max=0  help='gravity of the world' ");
	TwAddVarCB(bar, "air density", TW_TYPE_FLOAT, setDensityCB, getDensityCB, nullptr,
		" group='Mean' step=0.01 min =0  help='density of the air' ");
	TwAddVarCB(bar, "drag", TW_TYPE_FLOAT, setDragCB, getDragCB, nullptr,
		" group='Mean' step=0.01 min =0  help='drag coefficient of particle' ");
	TwAddVarCB(bar, "radius", TW_TYPE_FLOAT, setRadiusCB, getRadiusCB, nullptr,
		" group='Mean' step=0.01 min =0  help='radius of the particles' ");
	TwAddVarCB(bar, "elasticity", TW_TYPE_FLOAT, setRestCB, getRestCB, nullptr,
		" group='Mean' step=0.01 min =0  help='Collision elasticity of the particles' ");
	TwAddVarCB(bar, "friction", TW_TYPE_FLOAT, setFricCB, getFricCB, nullptr,
		" group='Mean' step=0.01 min =0  help='Collision friction of the particles' ");

	// variance variables
	TwAddVarCB(bar, "xPosVar", TW_TYPE_FLOAT, setxPosVarCB, getxPosVarCB, nullptr,
		" group='Variance' step=0.01 min=0 help='X Position variance of particle source' ");
	TwAddVarCB(bar, "yPosVar", TW_TYPE_FLOAT, setyPosVarCB, getyPosVarCB, nullptr,
		" group='Variance' step=0.01 min=0  help='Y Position variance of particle source' ");
	TwAddVarCB(bar, "zPosVar", TW_TYPE_FLOAT, setzPosVarCB, getzPosVarCB, nullptr,
		" group='Variance' step=0.01 min=0  help='Z Position variance of particle source' ");
	TwAddVarCB(bar, "xVelVar", TW_TYPE_FLOAT, setxVelVarCB, getxVelVarCB, nullptr,
		" group='Variance' step=0.01 min=0  help='X velocity variance of particle source' ");
	TwAddVarCB(bar, "yVelVar", TW_TYPE_FLOAT, setyVelVarCB, getyVelVarCB, nullptr,
		" group='Variance' step=0.01 min=0  help='Y velocity variance of particle source' ");
	TwAddVarCB(bar, "zVelVar", TW_TYPE_FLOAT, setzVelVarCB, getzVelVarCB, nullptr,
		" group='Variance' step=0.01 min=0  help='Z velocity variance of particle source' ");
	TwAddVarCB(bar, "lifeVar", TW_TYPE_INT32, setLifeVarCB, getLifeVarCB, nullptr,
		" group='Variance' step=1 min=1 help='Life variance of particle source' ");

	// Print OpenGL and GLSL versions.
	print_versions();
	// Setup callbacks.
	setup_callbacks(window);
	// Setup OpenGL settings.
	setup_opengl_settings();

	// Initialize the shader program; exit if initialization fails.
	if (!Window::initializeProgram()) exit(EXIT_FAILURE);
	// Initialize objects/pointers for rendering; exit if initialization fails.
	if (!Window::initializeObjects()) exit(EXIT_FAILURE);


	// Loop while GLFW window should stay open.
	while (!glfwWindowShouldClose(window))
	{
		// Main render display callback. Rendering of objects is done here.
		Window::displayCallback(window);

		// Draw tweak bars
		TwDraw();

		// Present frame buffer
		glfwSwapBuffers(window);

		// Idle callback. Updating objects, etc. can be done here.
		Window::idleCallback();
	}

	Window::cleanUp();
	// Destroy the window.
	glfwDestroyWindow(window);
	// Terminate tweakbar
	TwTerminate();
	// Terminate GLFW.
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setRateCB(const void* value, void* clientData) {
	Window::setFloatVariable("rate", *(const int*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getRateCB(void* value, void* clientData) {
	*static_cast<int*>(value) = Window::getFloatVariable("rate");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setxPosCB(const void* value, void* clientData) {
	Window::setFloatVariable("xPos", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getxPosCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("xPos");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setyPosCB(const void* value, void* clientData) {
	Window::setFloatVariable("yPos", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getyPosCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("yPos");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setzPosCB(const void* value, void* clientData) {
	Window::setFloatVariable("zPos", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getzPosCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("zPos");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setxVelCB(const void* value, void* clientData) {
	Window::setFloatVariable("xVel", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getxVelCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("xVel");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setyVelCB(const void* value, void* clientData) {
	Window::setFloatVariable("yVel", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getyVelCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("yVel");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setzVelCB(const void* value, void* clientData) {
	Window::setFloatVariable("zVel", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getzVelCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("zVel");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setLifeCB(const void* value, void* clientData) {
	Window::setFloatVariable("life", *(const int*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getLifeCB(void* value, void* clientData) {
	*static_cast<int*>(value) = Window::getFloatVariable("life");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setGravityCB(const void* value, void* clientData) {
	Window::setFloatVariable("gravity", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getGravityCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("gravity");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setDensityCB(const void* value, void* clientData) {
	Window::setFloatVariable("density", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getDensityCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("density");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setDragCB(const void* value, void* clientData) {
	Window::setFloatVariable("drag", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getDragCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("drag");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setRadiusCB(const void* value, void* clientData) {
	Window::setFloatVariable("radius", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getRadiusCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("radius");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setRestCB(const void* value, void* clientData) {
	Window::setFloatVariable("elasticity", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getRestCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("elasticity");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setFricCB(const void* value, void* clientData) {
	Window::setFloatVariable("friction", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getFricCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("friction");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setxPosVarCB(const void* value, void* clientData) {
	Window::setFloatVariable("xPosVar", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getxPosVarCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("xPosVar");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setyPosVarCB(const void* value, void* clientData) {
	Window::setFloatVariable("yPosVar", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getyPosVarCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("yPosVar");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setzPosVarCB(const void* value, void* clientData) {
	Window::setFloatVariable("zPosVar", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getzPosVarCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("zPosVar");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setxVelVarCB(const void* value, void* clientData) {
	Window::setFloatVariable("xVelVar", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getxVelVarCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("xVelVar");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setyVelVarCB(const void* value, void* clientData) {
	Window::setFloatVariable("yVelVar", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getyVelVarCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("yVelVar");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setzVelVarCB(const void* value, void* clientData) {
	Window::setFloatVariable("zVelVar", *(const float*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getzVelVarCB(void* value, void* clientData) {
	*static_cast<float*>(value) = Window::getFloatVariable("zVelVar");
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL setLifeVarCB(const void* value, void* clientData) {
	Window::setFloatVariable("lifeVar", *(const int*)value);
}

////////////////////////////////////////////////////////////////////////////////

void TW_CALL getLifeVarCB(void* value, void* clientData) {
	*static_cast<int*>(value) = Window::getFloatVariable("lifeVar");
}