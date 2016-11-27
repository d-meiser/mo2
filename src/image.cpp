#include <image.h>
#include <utilities.h>

namespace Mo {

class Image::Impl {};

Image::Image(const char* filename) {
  MO_UNUSED(filename);
}

Image::~Image() {}

}
