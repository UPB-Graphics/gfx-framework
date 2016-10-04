#pragma once

#define GL_ENABLED

#ifndef GL_DISABLE_GLEW
	#define GLEW_ENABLED
	#include <GL/glew.h>

	#ifdef _WIN32
		#include <GL/wglew.h>
	#endif

	#if defined(__linux__)
		#include <GL/glxew.h>
	#endif
#endif

#include <GLFW/glfw3.h>

// -------------------------------------------------------------------------
// Check for OpenGL Errors
// Returns 1 if an OpenGL error occurred, 0 otherwise.

#ifdef _DEBUG
#define CheckOpenGLError() OpenGL::CheckError(__FILE__, __LINE__)
#define SilentCheckOpenGLError() OpenGL::CheckError(__FILE__, __LINE__, false)
#else
#define CheckOpenGLError()
#define SilentCheckOpenGLError()
#endif

namespace OpenGL
{
	// -------------------------------------------------------------------------
	// Check for OpenGL Errors
	// Returns 1 if an OpenGL error occurred, 0 otherwise.
	int CheckError(const char *file, int line, bool log = true);
};