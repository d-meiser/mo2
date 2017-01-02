#include <tile.h>
#include <image.h>

namespace Mo {

Tile::Tile(float x, float y, float angle, float scale,
    std::unique_ptr<Image> image) :
    x_(x), y_(y), angle_(angle), scale_(scale),
    image_{std::move(image)} {}

Tile::Tile(const Tile& t) :
  x_(t.x_),
  y_(t.y_),
  angle_(t.angle_),
  scale_(t.scale_)
  {
  image_.reset(new Image(*t.image_));
}

Tile& Tile::operator=(const Tile& rhs) {
  if (this != &rhs) {
    x_ = rhs.x_;
    y_ = rhs.y_;
    angle_ = rhs.angle_;
    scale_ = rhs.scale_;
    image_.reset(new Image(*rhs.image_));
  }
  return *this;
}

float Tile::width() const {
  return image_->width() * scale_;
}

float Tile::height() const {
  return image_->height() * scale_;
}

}

