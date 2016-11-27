#ifndef IMAGE_H
#define IMAGE_H

#include <memory>
#include <vector>
#include <cstdint>


namespace Mo {

class Image {
public:
  typedef uint8_t Byte;

  Image(const char* filename);
  ~Image();
  void save(const char* filename);
  char *getPixelData();
  const char *getConstPixelData() const;
  int width() const;
  int height() const;
  int pitch() const;
  int numComponents() const;

private:
  int width_;
  int height_;
  int pitch_;
  int numComponents_;
  std::vector<Byte> pixelData_;

  void readJpegFile(const char *filename);
};

}

#endif

