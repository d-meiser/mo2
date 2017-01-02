#include <mosaic.h>
#include <algorithm>
#include <cmath>
#include <stdexcept>


namespace Mo {

Mosaic::Mosaic(
    const std::string& targetImage, float scale,
    const std::string* begin,
    const std::string* end) : targetImage_(targetImage, scale) {
  tiles_.reserve(std::distance(begin, end));
  std::for_each(begin, end,
      [this](const std::string& tileName) {
        tiles_.push_back(Tile{0, 0, 0, 1.0f,
                              std::unique_ptr<Image>(new Image(tileName))});
      });

  float area = 0.0f;
  for (const auto& t :tiles_) {
    area += t.width() * t.height();
  }

  float targetArea =
      scale * scale * targetImage_.width() * targetImage_.height();

  float tileScale = std::sqrt(targetArea / area);

  for (auto& t :tiles_) {
    t.scale_ = tileScale;
  }
}

Mosaic::Mosaic(
    const Image& targetImage,
    float scale,
    const std::string* begin,
    const std::string* end) : targetImage_(targetImage, scale) {
  tiles_.reserve(std::distance(begin, end));
  std::for_each(begin, end,
      [this](const std::string& tileName) {
        tiles_.push_back(Tile{0, 0, 0, 1.0f,
                              std::unique_ptr<Image>(new Image(tileName))});
      });
}

Mosaic::Mosaic(
    const Image& targetImage,
    float scale,
    const Tile* tileBegin, const Tile* tileEnd) :
  targetImage_(targetImage, scale),
  tiles_(tileBegin, tileEnd) {
}

Mosaic::Mosaic(
    const Image& targetImage,
    float scale) : targetImage_(targetImage, scale) {}

int Mosaic::size() const {
  return tiles_.size();
}

std::vector<Tile>::iterator Mosaic::tilesBegin() {
  return tiles_.begin();
}

std::vector<Tile>::iterator Mosaic::tilesEnd() {
  return tiles_.end();
}

std::vector<Tile>::const_iterator Mosaic::cTilesBegin() const {
  return tiles_.cbegin();
}

std::vector<Tile>::const_iterator Mosaic::cTilesEnd() const {
  return tiles_.cend();
}

const TargetImage& Mosaic::targetImage() const {
  return targetImage_;
}

void Mosaic::setTargetImage(const TargetImage& img) {
  targetImage_ = img;
}

void Mosaic::addTiles(const Tile* tileBegin, const Tile* tileEnd) {
  for (const Tile* t = tileBegin; t != tileEnd; ++t) {
    tiles_.emplace_back(Tile{t->x_, t->y_, t->angle_, t->scale_,
        std::unique_ptr<Image>(new Image(*t->image_))});
  }

  float totalTileArea = 0.0f;
  for (const auto& tile : tiles_) {
    totalTileArea += tile.width() * tile.height() * tile.scale_* tile.scale_;
  }

  float targetArea = targetImage_.image().width() *
      targetImage_.image().height() *
      targetImage_.scale() * targetImage_.scale();

  float rescaleFactor = std::sqrt(targetArea / totalTileArea);
  for (auto& t : tiles_) {
        t.scale_ *= 1.2f * rescaleFactor;
  }
}

void Mosaic::clear() {
  tiles_.clear();
}

void Mosaic::reduceSize(size_t newSize) {
  if (newSize > tiles_.size()) {
    throw std::runtime_error("newSize larger than current size.");
  }
  tiles_.resize(newSize);
}

const std::vector<Tile>& Mosaic::getTiles() const {
  return tiles_;
}

}
