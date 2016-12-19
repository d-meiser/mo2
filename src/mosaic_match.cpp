#include <mosaic_match.h>
#include <utilities.h>


namespace Mo {

class MosaicMatch::Impl : public Badness {
  public:
    Impl(std::shared_ptr<MosaicRenderer> renderer) {
      MO_UNUSED(renderer);
    }

    virtual float computeBadness(const Mosaic &model,
        const TargetImage &targetImage) {
      MO_UNUSED(model);
      MO_UNUSED(targetImage);
      return 0.0f;
    }
};

MosaicMatch::MosaicMatch(std::shared_ptr<MosaicRenderer> renderer) :
    impl_{new Mo::MosaicMatch::Impl{renderer}} {}

MosaicMatch::~MosaicMatch() {}

float MosaicMatch::computeBadness(const Mosaic& model,
    const TargetImage &targetImage) {
  MO_UNUSED(model);
  MO_UNUSED(targetImage);
  return 0.0f;
}


}

