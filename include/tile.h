#ifndef TILE_H
#define TILE_H

#include <memory>
#include <mo_lib_export.h>


namespace Mo {

class Image;

struct MO_LIB_EXPORT Tile {
  float x_;
  float y_;
  float angle_;
  float scale_;
  std::unique_ptr<Image> image_;
  float width() const;
  float height() const;
};

}

#endif
