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
  int bufferSize_;
  const Mosaic* mosaic_;
  GLuint vbo_;
  GLuint vao_;
#if 0
  GLuint tileTextures_;
#endif

  const char* vertexShaderSource() override;
  const char* fragmentShaderSource() override;
  void bindVAO() override;
  void draw() override;

  void setupVAO();
};

}

#endif

