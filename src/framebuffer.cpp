#include <framebuffer.h>
#include <stdexcept>

#define CHECK_GL_ERROR \
  err = glGetError(); \
  if (GL_NO_ERROR != err) { \
    throw std::runtime_error("OpenGL error"); \
  }

namespace Mo {

Framebuffer::Framebuffer(int width, int height) :
  width_(width),
  height_(height) {
  GLint err;
  CHECK_GL_ERROR;

  // Create FBO
  glGenFramebuffers(1, &fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  CHECK_GL_ERROR;

  // Create color texture
  glGenTextures(1, &colorTexture_);
  glBindTexture(GL_TEXTURE_2D, colorTexture_);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  CHECK_GL_ERROR;

  glGenTextures(1, &depthTexture_);
  glBindTexture(GL_TEXTURE_2D, depthTexture_);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, width, height); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  CHECK_GL_ERROR;

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
      colorTexture_, 0);
  CHECK_GL_ERROR;

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depthTexture_, 0);
  CHECK_GL_ERROR;

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("Framebuffer incomplete.");
  }

  static const GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buffers);
  CHECK_GL_ERROR;
}

Framebuffer::~Framebuffer() {
  glDeleteTextures(1, &colorTexture_);
  glDeleteTextures(1, &depthTexture_);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &fbo_);
}

size_t Framebuffer::size() const {
  return width_ * height_ * 4;
}

void Framebuffer::bind() {
  GLint err;
  CHECK_GL_ERROR;
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glViewport(0, 0, width_, height_);
  CHECK_GL_ERROR;
}

void Framebuffer::getPixels(unsigned char* data) {
  bind();
  GLint err;
  glReadPixels(0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, data);
  CHECK_GL_ERROR;
}

}
