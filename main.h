#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>


#include <stdlib.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include "Window.h"

#include "AntTweakBar.h"

// tweak bar setter declarations
void TW_CALL setRateCB(const void* value, void* clientData);
void TW_CALL setxPosCB(const void* value, void* clientData);
void TW_CALL setyPosCB(const void* value, void* clientData);
void TW_CALL setzPosCB(const void* value, void* clientData);
void TW_CALL setxVelCB(const void* value, void* clientData);
void TW_CALL setyVelCB(const void* value, void* clientData);
void TW_CALL setzVelCB(const void* value, void* clientData);
void TW_CALL setLifeCB(const void* value, void* clientData);
void TW_CALL setGravityCB(const void* value, void* clientData);
void TW_CALL setDensityCB(const void* value, void* clientData);
void TW_CALL setDragCB(const void* value, void* clientData);
void TW_CALL setRadiusCB(const void* value, void* clientData);
void TW_CALL setRestCB(const void* value, void* clientData);
void TW_CALL setFricCB(const void* value, void* clientData);
void TW_CALL setxPosVarCB(const void* value, void* clientData);
void TW_CALL setyPosVarCB(const void* value, void* clientData);
void TW_CALL setzPosVarCB(const void* value, void* clientData);
void TW_CALL setxVelVarCB(const void* value, void* clientData);
void TW_CALL setyVelVarCB(const void* value, void* clientData);
void TW_CALL setzVelVarCB(const void* value, void* clientData);
void TW_CALL setLifeVarCB(const void* value, void* clientData);

// tweak bar getter declarations
void TW_CALL getRateCB( void* value, void* clientData);
void TW_CALL getxPosCB( void* value, void* clientData);
void TW_CALL getyPosCB( void* value, void* clientData);
void TW_CALL getzPosCB( void* value, void* clientData);
void TW_CALL getxVelCB( void* value, void* clientData);
void TW_CALL getyVelCB( void* value, void* clientData);
void TW_CALL getzVelCB( void* value, void* clientData);
void TW_CALL getLifeCB( void* value, void* clientData);
void TW_CALL getGravityCB( void* value, void* clientData);
void TW_CALL getDensityCB( void* value, void* clientData);
void TW_CALL getDragCB( void* value, void* clientData);
void TW_CALL getRadiusCB( void* value, void* clientData);
void TW_CALL getRestCB( void* value, void* clientData);
void TW_CALL getFricCB( void* value, void* clientData);
void TW_CALL getxPosVarCB( void* value, void* clientData);
void TW_CALL getyPosVarCB( void* value, void* clientData);
void TW_CALL getzPosVarCB( void* value, void* clientData);
void TW_CALL getxVelVarCB( void* value, void* clientData);
void TW_CALL getyVelVarCB( void* value, void* clientData);
void TW_CALL getzVelVarCB( void* value, void* clientData);
void TW_CALL getLifeVarCB( void* value, void* clientData);

#endif