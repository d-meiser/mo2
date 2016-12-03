#include <tile.h>
#include <image.h>

namespace Mo {

float Tile::width() const {
  return image_->width() * scale_;
}

float Tile::height() const {
  return image_->height() * scale_;
}

}

