#pragma once

#ifndef GL_DISABLE_GLEW
#   define GLEW_ENABLED
#   include <GL/glew.h>
#endif


// -------------------------------------------------------------------------
// Check for OpenGL Errors
// Returns 1 if an OpenGL error occurred, 0 otherwise.

#ifdef _DEBUG
#   define CheckOpenGLError() gl_utils::CheckError(__FILE__, __LINE__)
#   define SilentCheckOpenGLError() gl_utils::CheckError(__FILE__, __LINE__, false)
#else
#   define CheckOpenGLError()
#   define SilentCheckOpenGLError()
#endif

namespace gl_utils
{
    // -------------------------------------------------------------------------
    // Check for OpenGL Errors
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    int CheckError(const char *file, int line, bool log = true);
}
