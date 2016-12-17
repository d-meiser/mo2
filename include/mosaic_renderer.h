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
  virtual void setMosaic(Mosaic* mosaic) = 0;
  virtual void setTileImages(const std::vector<Tile>& tiles) = 0;
};

}

#endif

