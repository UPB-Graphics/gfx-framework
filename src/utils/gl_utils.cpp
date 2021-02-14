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


int gl_utils::CheckError(const char *file, int line)
{
    GLenum errLast = GL_NO_ERROR;

    while (true)
    {
        GLenum err = glGetError();

        if (err == GL_NO_ERROR)
        {
            break;
        }

        /*
         *  Normally, the error is reset by the call to glGetError(), but
         *  if glGetError() itself returns an error, we risk looping forever
         *  here, so we check that we get a different error than the last
         *  time.
         */
        if (err == errLast)
        {
            std::cout << "OpenGL error state couldn't be reset";
            break;
        }

        errLast = err;

        PrintGLErrorDescription(err);
        std::cout << "\t[File] : " << file << "\t[Line] : " << line << std::endl;
    }

    return errLast;
}
