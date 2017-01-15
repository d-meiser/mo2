#include <utilities.h>
#include <framebuffer.h>
#include <image.h>
#include <mosaic.h>
#include <mosaic_renderer_textured.h>


namespace Mo {

void dumpFramebuffer(const Framebuffer& fb, const std::string& filename) {
  Image img(fb.width(), fb.height(), 4);
  const_cast<Framebuffer&>(fb).getPixels(img.getPixelData());
  img.save(filename);
}


void dumpMosaic(const Mosaic& mosaic, const std::string& filename) {
  int width = mosaic.targetImage().width();
  int height = mosaic.targetImage().height();
  MosaicRendererTextured renderer{width, height};
  renderer.setMosaic(mosaic);
  renderer.setTileImages(mosaic.getTiles());
  Framebuffer fb{width, height};
  fb.bind();
  renderer.render();
  dumpFramebuffer(fb, filename);
}

}

