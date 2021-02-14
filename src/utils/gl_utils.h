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


// Use discrete GPU by default. See [1] for Nvidia and [2] for AMD.
//
// [1] http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
// [2] https://gpuopen.com/amdpowerxpressrequesthighperformance/
#if defined(_WIN32)
#   define PREFER_DISCRETE_GPU_NVIDIA   extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 1
#   define PREFER_DISCRETE_GPU_AMD      extern "C" __declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 1
#else
#   define PREFER_DISCRETE_GPU_NVIDIA
#   define PREFER_DISCRETE_GPU_AMD
#endif
