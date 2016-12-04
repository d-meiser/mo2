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
  CHECK_GL_ERROR;
}

Framebuffer::~Framebuffer() {}

}
