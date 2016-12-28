#include <interactiontiletile.h>
#include <mosaic.h>
#include <potential.h>
#include <interactionhelpers.h>
#include <utilities.h>


namespace Mo {

InteractionTileTile::InteractionTileTile(
    std::unique_ptr<Potential> potential) : potential_(std::move(potential)) {
}

float InteractionTileTile::computeBadness(const Mosaic &model) {

  float badness = 0.0f;
  auto begin = model.cTilesBegin();
  auto end = model.cTilesEnd();
  for (auto t1 = begin; t1 != end; ++t1) {
    for (auto t2 = begin; t2 != t1; ++t2) {
      badness += computeBadnessPair(
          t1->x_, t1->y_, t1->image_->width(), t1->image_->height(),
          t1->angle_, t1->scale_,
          t2->x_, t2->y_, t2->image_->width(), t2->image_->height(),
          t2->angle_, t2->scale_,
          potential_.get());
    }
  }
  return badness;
}

void InteractionTileTile::resetPotential(std::unique_ptr<Potential> potential) {
  potential_ = std::move(potential);
}

}
