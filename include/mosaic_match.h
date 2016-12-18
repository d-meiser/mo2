#ifndef MOSAIC_MATCH_H
#define MOSAIC_MATCH_H

#include <badness.h>
#include <mo_lib_export.h>
#include <memory>


namespace Mo {

class Renderer;

class MO_LIB_EXPORT MosaicMatch : public Badness {
  public:
    MosaicMatch(std::shared_ptr<Renderer> renderer);
    virtual float computeBadness(const Mosaic &model,
        const TargetImage &targetImage);

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}
#endif

