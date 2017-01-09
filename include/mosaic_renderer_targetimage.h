#ifndef MOSAIC_RENDERER_TARGETIMAGE_H
#define MOSAIC_RENDERER_TARGETIMAGE_H

#include <mo_lib_export.h>
#include <mosaic_renderer.h>
#include <GL/gl3w.h>


namespace Mo {

class MO_LIB_EXPORT MosaicRendererTargetImage : public MosaicRenderer {
public:
  MosaicRendererTargetImage(int renderTargetWidth, int renderTargetHeight);
  ~MosaicRendererTargetImage() override;
  void setMosaic(const Mosaic& mosaic) override;
  void setTileImages(const std::vector<Tile>& tiles) override;

private:
  GLuint targetImageTexture_;
  GLuint vao_;

  const char* vertexShaderSource() override;
  const char* fragmentShaderSource() override;
  void bindVAO() override;
  void draw() override;
};

}

#endif

