#ifndef MOSAIC_MATCH_H
#define MOSAIC_MATCH_H

#include <badness.h>
#include <mo_lib_export.h>
#include <memory>


namespace Mo {

class MosaicRenderer;

class MO_LIB_EXPORT MosaicMatch : public Badness {
  public:
    MosaicMatch(std::shared_ptr<MosaicRenderer> renderer);
    ~MosaicMatch();
    virtual float computeBadness(const Mosaic &model);

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}
#endif

