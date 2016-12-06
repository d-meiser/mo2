#ifndef UTILITIES_H
#define UTILITIES_H

#include <GL/gl3w.h>
#include <sstream>
#include <stdexcept>


#define MO_UNUSED(x) (void)(x)

static inline void moCheckGlError(const char* file, int line) {
  GLint err = glGetError();
  if (GL_NO_ERROR != err) { \
    std::stringstream ss("OpenGL error at ");
    ss << file;
    ss << "(" << line << "): ";
    ss << err;
    throw std::runtime_error(ss.str()); 
  }
}

#define MO_CHECK_GL_ERROR moCheckGlError(__FILE__, __LINE__)

#endif

