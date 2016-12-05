#ifndef MOSAIC_RENDERER_OUTLINE_H
#define MOSAIC_RENDERER_OUTLINE_H

#include <mo_lib_export.h>
#include <mosaic_renderer.h>


namespace Mo {

class MO_LIB_EXPORT MosaicRendererOutline : public MosaicRenderer {
public:
  ~MosaicRendererOutline();

private:
  Mosaic* mosaic_;

  void setMosaic(Mosaic* mosaic) override;
  const char* vertexShaderSource() override;
  const char* fragmentShaderSource() override;
  void bindVAO() override;
  void draw() override;
};

}

#endif

