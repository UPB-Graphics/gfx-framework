#pragma once

#include <GL/glew.h>
#undef APIENTRY


// -------------------------------------------------------------------------
// Macros to check and retrieve OpenGL errors

#ifdef DEBUG
#   define CheckOpenGLError()           gl_utils::CheckError(__FILE__, __LINE__)
#   define GetOpenGLError()             gl_utils::CheckError(__FILE__, __LINE__)
#else
#   define CheckOpenGLError()
#   define GetOpenGLError()             GL_NO_ERROR
#endif


namespace gl_utils
{
    // -------------------------------------------------------------------------
    // Check for OpenGL Errors
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    int CheckError(const char *file, int line);
}


// Use discrete GPU by default. See [1] for Nvidia and [2] for AMD.
//
// [1] http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
// [2] https://gpuopen.com/amdpowerxpressrequesthighperformance/
#define PREFER_DISCRETE_GPU_NVIDIA      extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1
#define PREFER_DISCRETE_GPU_AMD         extern "C" __declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 1
