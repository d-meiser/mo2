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
  void bind();
  size_t size() const;
  void getPixels(unsigned char* data);
  int width() const;
  int height() const;

private:
  GLuint fbo_;
  GLuint colorTexture_;
  GLuint depthTexture_;
  int width_;
  int height_;
};

}

#endif

