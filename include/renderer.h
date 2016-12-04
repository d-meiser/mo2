#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <mo_lib_export.h>
#include <string>


namespace Mo {

class MO_LIB_EXPORT Renderer {
public:
  Renderer();
  virtual ~Renderer();

  void render();

private:
  std::unique_ptr<int> shaderProgram_;
  virtual std::string vertexShaderSource() = 0;
  virtual std::string fragmentShaderSource() = 0;
};

}

#endif

