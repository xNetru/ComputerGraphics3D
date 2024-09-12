#include "Common.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    // GL_NO_ERROR is guaranteed to be equal 0.
    bool result = true;
    while (GLenum error = glGetError())
    {
        if (result)
        {
            std::cerr << "Function: " << function << std::endl;
            std::cerr << "File: " << file << std::endl;
            std::cerr << "Line: " << line << std::endl;
        }
        std::cerr << "[OpenGL error] (" << error << ")" << std::endl;
        result = false;
    }
    return result;
}
