#include <renderer.h>
#include <GL/gl3w.h>
#include <stdexcept>
#include <utilities.h>
#include <string>
#include <sstream>


namespace Mo {


static void checkShaderCompilationStatus(GLuint shader, GLenum shaderType);

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
  glUseProgram(shaderProgram_);
  bindVAO();
  draw();
}

void Renderer::createShaderProgram() {
  MO_CHECK_GL_ERROR;

  GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vSources[] = {vertexShaderSource()};
  glShaderSource(vertexShader, 1, vSources, 0);
  glCompileShader(vertexShader);
  MO_CHECK_GL_ERROR;
  checkShaderCompilationStatus(vertexShader, GL_VERTEX_SHADER);

  GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fSources[] = {fragmentShaderSource()};
  glShaderSource(fragmentShader, 1, fSources, 0);
  glCompileShader(fragmentShader);
  MO_CHECK_GL_ERROR;
  checkShaderCompilationStatus(vertexShader, GL_FRAGMENT_SHADER);

  shaderProgram_ = glCreateProgram();
  glAttachShader(shaderProgram_, vertexShader);
  glAttachShader(shaderProgram_, fragmentShader);
  glLinkProgram(shaderProgram_);
  MO_CHECK_GL_ERROR;

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  MO_CHECK_GL_ERROR;

}

void checkShaderCompilationStatus(GLuint shader, GLenum shaderType) {
  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (GL_FALSE == success) {
    std::stringstream errorMessage;
    errorMessage << "Failed to compile ";
    switch (shaderType) {
      case GL_VERTEX_SHADER:
          errorMessage << "vertex";
          break;
      case GL_FRAGMENT_SHADER:
          errorMessage << "fragment";
          break;
      default:
          break;
    }
    errorMessage << " shader.\n";
    errorMessage << "Compiler log:\n";

    GLint maxLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    std::string infoLog;
    infoLog.resize(maxLength);
    glGetShaderInfoLog(shader, maxLength, 0, &infoLog[0]);
    errorMessage << infoLog << std::endl;

    throw std::runtime_error(errorMessage.str());
  }
}


}

