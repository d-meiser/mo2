#include <mosaic_match.h>
#include <utilities.h>
#include <framebuffer.h>
#include <mosaic_renderer.h>
#include <mosaic.h>
#include <iostream>


namespace Mo {

namespace {

template<typename T>
float sqr_difference(T a, T b) {
  float tmp = static_cast<float>(a) - static_cast<float>(b);
  return tmp * tmp;
}

}

class MosaicMatch::Impl : public Badness {
  public:
    Impl(std::shared_ptr<MosaicRenderer> renderer) :
      renderer_(renderer) {
    }

    float computeBadness(const Mosaic &model) override {
      ensureFrameBufferSizeIsCorrect(model.targetImage());

      renderer_->setMosaic(model);
      renderer_->setTileImages(model.getTiles());
      framebuffer_->bind();
      renderer_->setRenderTargetWidth(framebuffer_->width());
      renderer_->setRenderTargetHeight(framebuffer_->height());
      renderer_->render();

      MO_ASSERT(framebuffer_->size() > 0);
      const auto& targetImage = model.targetImage();
      Image renderedImage(targetImage.image().width(),
                          targetImage.image().height(), 4);
      framebuffer_->getPixels(renderedImage.getPixelData());
      renderedImage.setQuality(100);
      renderedImage.save("renderedImage.jpg");

      return renderedImage.distance(targetImage.image());
    }

  private:
    std::shared_ptr<MosaicRenderer> renderer_;
    std::unique_ptr<Framebuffer> framebuffer_;

    void ensureFrameBufferSizeIsCorrect(const TargetImage& targetImage) {
      int width = targetImage.image().width();
      int height = targetImage.image().height();
      if (!framebuffer_ ||
          width != framebuffer_->width() ||
          height != framebuffer_->height()) {
        framebuffer_.reset(
            new Framebuffer(width, height));
      }
    }
};

MosaicMatch::MosaicMatch(std::shared_ptr<MosaicRenderer> renderer) :
    impl_{new Mo::MosaicMatch::Impl{renderer}} {}

MosaicMatch::~MosaicMatch() {}

float MosaicMatch::computeBadness(const Mosaic& model) {
  return impl_->computeBadness(model);
}

}

