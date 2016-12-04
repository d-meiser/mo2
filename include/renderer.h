#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <mo_lib_export.h>
#include <string>


namespace Mo {

class MO_LIB_EXPORT Renderer {
public:
  Renderer();
  Renderer(const Renderer&) = delete;
  const Renderer& operator=(const Renderer&) = delete;
  virtual ~Renderer();

  void render();

private:
  int shaderProgram_;
  virtual const char* vertexShaderSource() = 0;
  virtual const char* fragmentShaderSource() = 0;

  void createShaderProgram();
};

}

#endif

