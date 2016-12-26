#include <mosaic_match.h>
#include <utilities.h>
#include <framebuffer.h>
#include <mosaic_renderer.h>
#include <mosaic.h>


namespace Mo {

class MosaicMatch::Impl : public Badness {
  public:
    Impl(std::shared_ptr<MosaicRenderer> renderer) :
      renderer_(renderer) {
    }

    float computeBadness(const Mosaic &model,
        const TargetImage &targetImage) override {
      MO_UNUSED(targetImage);
      renderer_->setMosaic(model);
      renderer_->setTileImages(model.getTiles());
      return 0.0f;
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

