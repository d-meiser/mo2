#include <mosaic_match.h>
#include <utilities.h>
#include <framebuffer.h>
#include <mosaic_renderer.h>
#include <mosaic_renderer_targetimage.h>
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
      renderer_(renderer),
      targetImageRenderer_(nullptr),
      targetImageWidth_(0),
      targetImageHeight_(0) {
    }

    float computeBadness(const Mosaic &mosaic) override {
      ensureFrameBufferSizeIsCorrect(mosaic.targetImage());
      MO_ASSERT(framebuffer_->size() > 0);
      if (targetImageWidth_ != framebuffer_->width() ||
          targetImageHeight_ != framebuffer_->height()) {
        targetImageRenderer_.reset(
            new MosaicRendererTargetImage(framebuffer_->width(),
                                          framebuffer_->height()));
        targetImageWidth_ = framebuffer_->width();
        targetImageHeight_ = framebuffer_->height();
      }

      framebuffer_->bind();

      Image renderedMosaic{renderMosaic(renderer_.get(), mosaic)};
      renderedMosaic.save("renderedMosaic.jpg");

      targetImageRenderer_->setRenderTargetWidth(framebuffer_->width());
      targetImageRenderer_->setRenderTargetHeight(framebuffer_->height());
      targetImageRenderer_->render();

      Image targetImage{renderMosaic(targetImageRenderer_.get(), mosaic)};
      targetImage.save("targetImage.jpg");

      return renderedMosaic.distance(targetImage);
    }

  private:
    std::shared_ptr<MosaicRenderer> renderer_;
    std::unique_ptr<MosaicRendererTargetImage> targetImageRenderer_;
    std::unique_ptr<Framebuffer> framebuffer_;
    int targetImageWidth_;
    int targetImageHeight_;


    void ensureFrameBufferSizeIsCorrect(const TargetImage& targetImage) {
      int width = targetImage.width();
      int height = targetImage.height();
      if (!framebuffer_ ||
          width != framebuffer_->width() ||
          height != framebuffer_->height()) {
        framebuffer_.reset(
            new Framebuffer(width, height));
      }
    }

    Image renderMosaic(MosaicRenderer* renderer, const Mosaic& mosaic) {
      renderer->setMosaic(mosaic);
      renderer->setTileImages(mosaic.getTiles());
      renderer->setRenderTargetWidth(framebuffer_->width());
      renderer->setRenderTargetHeight(framebuffer_->height());
      renderer->render();

      Image img(framebuffer_->width(), framebuffer_->height(), 4);
      framebuffer_->getPixels(img.getPixelData());
      return img;
    }
};

MosaicMatch::MosaicMatch(std::shared_ptr<MosaicRenderer> renderer) :
    impl_{new Mo::MosaicMatch::Impl{renderer}} {}

MosaicMatch::~MosaicMatch() {}

float MosaicMatch::computeBadness(const Mosaic& model) {
  return impl_->computeBadness(model);
}

}

