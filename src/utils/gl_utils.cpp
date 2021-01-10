#include "utils/gl_utils.h"

#include <string>
#include <iostream>


void PrintGLErrorDescription(unsigned int glErr)
{
    static std::string GLerrorDescription[] = {
        "GL_INVALID_ENUM",                      // 0x0500
        "GL_INVALID_VALUE",                     // 0x0501
        "GL_INVALID_OPERATION",                 // 0x0502
        "GL_STACK_OVERFLOW",                    // 0x0503
        "GL_STACK_UNDERFLOW",                   // 0x0504
        "GL_OUT_OF_MEMORY",                     // 0x0505
        "GL_INVALID_FRAMEBUFFER_OPERATION"      // 0x0506
    };

    std::cout << "\n[OpenGL Error]\n";
    std::cout << "\t[" << glErr << "] : " << GLerrorDescription[glErr - GL_INVALID_ENUM].c_str() << "\n";
}


int gl_utils::CheckError(const char * file, int line, bool log)
{
    #ifdef _DEBUG
        GLenum glErr = glGetError();
        if (glErr != GL_NO_ERROR && log)
        {
            PrintGLErrorDescription(glErr);
            std::cout << "\t[File] : " << file << "\t[Line] : " << line << std::endl;
            return glErr;
        }
    #endif
    return 0;
}
