#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <mo_lib_export.h>
#include <string>


namespace Mo {

class MO_LIB_EXPORT Renderer {
public:
  Renderer(int renderTargetWidth, int renderTargetHeight);
  Renderer(const Renderer&) = delete;
  const Renderer& operator=(const Renderer&) = delete;
  virtual ~Renderer();

  void render();

  int renderTargetWidth() const;
  void setRenderTargetWidth(int renderTargetWidth);
  int renderTargetHeight() const;
  void setRenderTargetHeight(int renderTargetHeight);

private:
  int shaderProgram_;
  virtual const char* vertexShaderSource() = 0;
  virtual const char* fragmentShaderSource() = 0;
  virtual void bindVAO() = 0;
  virtual void draw() = 0;

  int renderTargetWidth_;
  int renderTargetHeight_;

  void createShaderProgram();
};

}

#endif

