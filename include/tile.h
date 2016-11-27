#ifndef TILE_H
#define TILE_H

namespace Mo {

class Image;

struct Tile {
  float x_;
  float y_;
  float angle_;
  float scale_;
  Image* image_;
};

}

#endif
