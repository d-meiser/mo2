#ifndef MOSAIC_RENDERER_H
#define MOSAIC_RENDERER_H

#include <mo_lib_export.h>
#include <renderer.h>
#include <tile.h>
#include <vector>


namespace Mo {

class Mosaic;

class MO_LIB_EXPORT MosaicRenderer : public Renderer {
public:
  MosaicRenderer(int renderTargetWidth, int renderTargetHeight) :
    Renderer(renderTargetWidth, renderTargetHeight) {}
  virtual void setMosaic(const Mosaic& mosaic) = 0;
  virtual void setTileImages(const std::vector<Tile>& tiles) = 0;
};

}

#endif

