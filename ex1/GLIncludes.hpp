#pragma once

#ifdef __APPLE__
#include <GL/glew.h>
#include <OpenGL/gl.h>
#elif _WIN32
#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext.hpp>
