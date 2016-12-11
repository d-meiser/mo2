#ifndef MOSAIC_H
#define MOSAIC_H

#include <memory>
#include <vector>
#include <tile.h>
#include <targetimage.h>
#include <mo_lib_export.h>


namespace Mo {

class MO_LIB_EXPORT Mosaic {
public:
  Mosaic(const std::string& targetImage, float scale,
      const std::string* tileBegin, const std::string* tileEnd);
  Mosaic(const Image& targetImage, float scale,
      const std::string* tileBegin, const std::string* tileEnd);
  Mosaic(const Image& targetImage, float scale,
      const Tile* tileBegin, const Tile* tileEnd);
  Mosaic(const Image& targetImage, float scale);
  int size() const;
  std::vector<Tile>::iterator tilesBegin();
  std::vector<Tile>::iterator tilesEnd();
  std::vector<Tile>::const_iterator cTilesBegin() const;
  std::vector<Tile>::const_iterator cTilesEnd() const;
  const TargetImage& targetImage() const;
  void addTiles(const Tile* tileBegin, const Tile* tileEnd);
  void clear();
  void reduceSize(size_t newSize);
  const std::vector<Tile>& getTiles() const;

private:
  TargetImage targetImage_;
  std::vector<Tile> tiles_;
};

}

#endif

