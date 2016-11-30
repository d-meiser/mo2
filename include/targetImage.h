#ifndef TARGET_IMAGE_H
#define TARGET_IMAGE_H

namespace Mo {

class TargetImage {
public:
  TargetImage(const Image& image, float scale) :
    image_(image), scale_(scale) {}
  float scale() const { return scale_; }
  const Image& image() const { return image_; }

private:
  Image image_;
  float scale_;
};

}

#endif

