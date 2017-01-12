#ifndef UTILITIES_H
#define UTILITIES_H

#include <GL/gl3w.h>
#include <sstream>
#include <stdexcept>
#include <cassert>
#include <mo_lib_export.h>


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

#define MO_ASSERT(x) assert((x))


namespace Mo {

class Framebuffer;
class Mosaic;

void MO_LIB_EXPORT
  dumpFramebuffer(const Framebuffer& fb, const std::string& filename);
void MO_LIB_EXPORT
  dumpMosaic(const Mosaic& mosaic, const std::string& filename);
void MO_LIB_EXPORT
  dumpMosaicTargetImage(const Mosaic& mosaic, const std::string& filename);
void MO_LIB_EXPORT
  dumpMosaicOutlines(const Mosaic& mosaic, const std::string& filename);
void MO_LIB_EXPORT
  dumpMosaicTiles(const Mosaic& mosaic, const std::string& filename);

}


#endif

