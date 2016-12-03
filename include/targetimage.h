#ifndef TARGET_IMAGE_H
#define TARGET_IMAGE_H

#include <image.h>


namespace Mo {

class TargetImage {
public:
  TargetImage(const Image& image, float scale) :
    image_(image), scale_(scale) {}

  float scale() const {
    return scale_;
  }

  const Image& image() const {
    return image_;
  }

  float width() const {
    return image_.width() * scale_;
  }

  float height() const {
    return image_.height() * scale_;
  }

private:
  Image image_;
  float scale_;
};

}

#endif

