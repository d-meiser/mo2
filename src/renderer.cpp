#include <renderer.h>
#include <GL/gl3w.h>
#include <stdexcept>
#include <utilities.h>


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
  MO_CHECK_GL_ERROR;

  GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vSources[] = {vertexShaderSource()};
  glShaderSource(vertexShader, 1, vSources, 0);
  glCompileShader(vertexShader);
  MO_CHECK_GL_ERROR;

  GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fSources[] = {fragmentShaderSource()};
  glShaderSource(fragmentShader, 1, fSources, 0);
  glCompileShader(fragmentShader);
  MO_CHECK_GL_ERROR;

  shaderProgram_ = glCreateProgram();
  glAttachShader(shaderProgram_, vertexShader);
  glAttachShader(shaderProgram_, fragmentShader);
  glLinkProgram(shaderProgram_);
  MO_CHECK_GL_ERROR;

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  MO_CHECK_GL_ERROR;
}

}

