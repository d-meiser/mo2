#include <mosaic_renderer_outline.h>

namespace Mo {

MosaicRendererOutline::~MosaicRendererOutline() {}

void MosaicRendererOutline::setMosaic(Mosaic* mosaic) {
  mosaic_ = mosaic;
}

const char* MosaicRendererOutline::vertexShaderSource() {
  return "";
}

const char* MosaicRendererOutline::fragmentShaderSource() {
  return "";
}

void MosaicRendererOutline::bindVAO() {
}

void MosaicRendererOutline::draw() {
}

}

