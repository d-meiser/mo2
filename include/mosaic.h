#ifndef MOSAIC_H
#define MOSAIC_H

#include <memory>
#include <vector>
#include <tile.h>
#include <image.h>
#include <mo_lib_export.h>


namespace Mo {

class MO_LIB_EXPORT Mosaic {
public:
  Mosaic(const std::string& targetImage,
      const std::string* tileBegin, const std::string* tileEnd);

private:
  Image targetImage_;
  std::vector<Tile> tiles_;
};

}

#endif

