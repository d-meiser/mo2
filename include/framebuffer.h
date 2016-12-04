#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <mo_lib_export.h>


namespace Mo {

class MO_LIB_EXPORT Framebuffer {
public:
  Framebuffer(int width, int height);
  ~Framebuffer();

private:
  GLuint fbo_;
  GLuint colorTexture_;
  GLuint depthStencilTexture_;
};

}

#endif

