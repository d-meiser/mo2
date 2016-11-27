#ifndef TILE_H
#define TILE_H

#include <memory>


namespace Mo {

class Image;

struct Tile {
  float x_;
  float y_;
  float angle_;
  float scale_;
  std::unique_ptr<Image> image_;
};

}

#endif
