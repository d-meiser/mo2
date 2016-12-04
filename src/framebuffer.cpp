#include <framebuffer.h>
#include <stdexcept>

#define CHECK_GL_ERROR \
  err = glGetError(); \
  if (GL_NO_ERROR != err) { \
    throw std::runtime_error("OpenGL error"); \
  }

namespace Mo {

Framebuffer::Framebuffer(int width, int height) {
  GLint err;
  CHECK_GL_ERROR;

  // Create FBO
  glGenFramebuffers(1, &fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  CHECK_GL_ERROR;

  // Create backing texture
  glGenTextures(1, &colorTexture_);
  glBindTexture(GL_TEXTURE_2D, colorTexture_);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  CHECK_GL_ERROR;

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
      colorTexture_, 0);
  CHECK_GL_ERROR;
}

Framebuffer::~Framebuffer() {}

}
