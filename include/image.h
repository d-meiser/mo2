#ifndef IMAGE_H
#define IMAGE_H

#include <memory>
#include <vector>
#include <cstdint>

#include <mo_lib_export.h>


namespace Mo {

class MO_LIB_EXPORT Image {
public:
  typedef uint8_t Byte;

  Image(const char* filename);
  Image(int width, int height);
  ~Image();
  void save(const char* filename);
  char *getPixelData();
  const char *getConstPixelData() const;
  int width() const;
  int height() const;
  int pitch() const;
  int numComponents() const;
  void setQuality(int quality);
  int quality() const;

private:
  int width_;
  int height_;
  int pitch_;
  int numComponents_;
  int quality_;
  std::vector<Byte> pixelData_;

  void readJpegFile(const char *filename);
  void saveJpegFile(const char *filename);
};

}

#endif

