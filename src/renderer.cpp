#include <renderer.h>
#include <GL/gl3w.h>
#include <stdexcept>

#define CHECK_GL_ERROR \
  err = glGetError(); \
  if (GL_NO_ERROR != err) { \
    throw std::runtime_error("OpenGL error"); \
  }


namespace Mo {

Renderer::Renderer() : shaderProgram_(0) {}

Renderer::~Renderer() {
  if (shaderProgram_) {
    glDeleteProgram(shaderProgram_);
  }
}

void Renderer::render() {
  if (!shaderProgram_) {
    createShaderProgram();
  }
  bindVAO();
  glUseProgram(shaderProgram_);
  draw();
}

void Renderer::createShaderProgram() {
  GLint err;
  CHECK_GL_ERROR;

  GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vSources[] = {vertexShaderSource()};
  glShaderSource(vertexShader, 1, vSources, 0);
  glCompileShader(vertexShader);
  CHECK_GL_ERROR;

  GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fSources[] = {fragmentShaderSource()};
  glShaderSource(fragmentShader, 1, fSources, 0);
  glCompileShader(fragmentShader);
  CHECK_GL_ERROR;

  shaderProgram_ = glCreateProgram();
  glAttachShader(shaderProgram_, vertexShader);
  glAttachShader(shaderProgram_, fragmentShader);
  glLinkProgram(shaderProgram_);
  CHECK_GL_ERROR;

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  CHECK_GL_ERROR;
}

}

