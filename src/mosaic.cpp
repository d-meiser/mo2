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

}
