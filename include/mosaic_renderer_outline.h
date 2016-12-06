#ifndef MOSAIC_RENDERER_OUTLINE_H
#define MOSAIC_RENDERER_OUTLINE_H

#include <mo_lib_export.h>
#include <mosaic_renderer.h>
#include <GL/gl3w.h>


namespace Mo {

class MO_LIB_EXPORT MosaicRendererOutline : public MosaicRenderer {
public:
  MosaicRendererOutline();
  ~MosaicRendererOutline();
  void setMosaic(Mosaic* mosaic) override;

private:
  int size_;
  GLuint vbo_;
  GLuint vao_;
  GLuint tileTextures_;

  const char* vertexShaderSource() override;
  const char* fragmentShaderSource() override;
  void bindVAO() override;
  void draw() override;
};

}

#endif

