#include <mosaic.h>
#include <algorithm>


namespace Mo {

Mosaic::Mosaic(
    const std::string& targetImage,
    const std::string* begin,
    const std::string* end) : targetImage_(targetImage) {
  tiles_.reserve(std::distance(begin, end));
  std::for_each(begin, end,
      [this](const std::string& tileName) {
        tiles_.push_back(Tile{0, 0, 0, 1.0f,
                              std::unique_ptr<Image>(new Image(tileName))});
      });
}

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

}