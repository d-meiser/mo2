#ifndef IMAGE_H
#define IMAGE_H

#include <memory>


namespace Mo {

class Image {
public:
  Image(const char* filename);
  void save(const char* filename);
  char *getPixelData();
  const char *getConstPixelData() const;
  int width() const;
  int height() const;
  int pitch() const;

private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}

#endif

