#include <utilities.h>
#include <framebuffer.h>
#include <image.h>

namespace Mo {

void dumpFramebuffer(const Framebuffer& fb, const std::string& filename) {
  Image img(fb.width(), fb.height(), 4);
  const_cast<Framebuffer&>(fb).getPixels(img.getPixelData());
  img.save(filename);
}

}

