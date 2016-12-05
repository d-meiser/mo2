#ifndef MOSAIC_RENDERER_H
#define MOSAIC_RENDERER_H

#include <mo_lib_export.h>
#include <renderer.h>


namespace Mo {

class Mosaic;

class MO_LIB_EXPORT MosaicRenderer : public Renderer {
private:
  virtual void setMosaic(Mosaic* mosaic);
};

}

#endif

