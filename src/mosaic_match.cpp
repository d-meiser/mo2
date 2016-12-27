#include <mosaic_match.h>
#include <utilities.h>
#include <framebuffer.h>
#include <mosaic_renderer.h>
#include <mosaic.h>


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

    float computeBadness(const Mosaic &model,
        const TargetImage &targetImage) override {
      // Make sure framebuffer has right size
      if (!framebuffer_ ||
          targetImage.width() != framebuffer_->width() ||
          targetImage.height() != framebuffer_->height()) {
        framebuffer_.reset(
            new Framebuffer(targetImage.width(),
                            targetImage.height()));
      }

      renderer_->setMosaic(model);
      renderer_->setTileImages(model.getTiles());
      framebuffer_->bind();
      renderer_->render();

      MO_ASSERT(framebuffer_->size() > 0);
      std::vector<unsigned char> pixels(framebuffer_->size());
      framebuffer_->getPixels(&pixels[0]);
      int n = targetImage.width() * targetImage.height() * 3;
      const unsigned char* targetPixels =
          targetImage.image().getConstPixelData();
      float err = 0.0f;
      for (int i = 0; i < n; ++i) {
        err += sqr_difference(targetPixels[i], pixels[i]);
      }

      return err;
    }

  private:
    std::shared_ptr<MosaicRenderer> renderer_;
    std::unique_ptr<Framebuffer> framebuffer_;
};

MosaicMatch::MosaicMatch(std::shared_ptr<MosaicRenderer> renderer) :
    impl_{new Mo::MosaicMatch::Impl{renderer}} {}

MosaicMatch::~MosaicMatch() {}

float MosaicMatch::computeBadness(const Mosaic& model,
    const TargetImage &targetImage) {
  return impl_->computeBadness(model, targetImage);
}

}

