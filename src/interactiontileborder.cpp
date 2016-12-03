#include <interactiontileborder.h>
#include <potential.h>
#include <mosaic.h>
#include <interactionhelpers.h>
#include <targetimage.h>

#include <vector>
#include <cmath>


namespace Mo {

InteractionTileBorder::InteractionTileBorder(
        std::unique_ptr<Potential> potential) :
    potential_(std::move(potential)) {
}

float InteractionTileBorder::computeBadness(
    const Mosaic &mosaic, const TargetImage &targetImage) {
  // Number of effective tiles used to simulate interaction with border
  int numTilesLinear = static_cast<int>(
      std::round(std::sqrt(static_cast<float>(mosaic.size()))));
  const int minimumNumTiles = 4;
  numTilesLinear = std::max(minimumNumTiles, numTilesLinear);

  float badness = 0.0;

  // bottom row of fake tiles
  float width0 = static_cast<float>(targetImage.width()) / numTilesLinear;
  float height0 = static_cast<float>(targetImage.height()) / numTilesLinear;
  for (int i = 0; i != numTilesLinear; ++i) {
    int j = -1;
    float x0 = (i + 0.5) * width0;
    float y0 = (j + 0.5) * height0;
    // World coordinates have origin at center.  So shift by targetSize / 2
    x0 -= 0.5f * targetImage.width();
    y0 -= 0.5f * targetImage.height();
    for (auto t = mosaic.cTilesBegin(); t != mosaic.cTilesEnd(); ++ t) {
      badness +=
        computeBadnessPair(x0, y0, width0, height0, 0.0f, 1.0f,
            t->x_, t->y_, t->image_->width(), t->image_->height(),
            t->angle_, t->scale_,
            potential_.get());
    }
  }

  // right column of fake tiles
  for (int j = 0; j != numTilesLinear; ++j) {
    int i = numTilesLinear;
    float x0 = (i + 0.5) * width0;
    float y0 = (j + 0.5) * height0;
    // World coordinates have origin at center.  So shift by targetSize / 2
    x0 -= 0.5f * targetImage.width();
    y0 -= 0.5f * targetImage.height();
    for (auto t = mosaic.cTilesBegin(); t != mosaic.cTilesEnd(); ++ t) {
      badness +=
        computeBadnessPair(x0, y0, width0, height0, 0.0f, 1.0f,
            t->x_, t->y_, t->image_->width(), t->image_->height(),
            t->angle_, t->scale_,
            potential_.get());
    }
  }

  // top row of fake tiles
  for (int i = 0; i != numTilesLinear; ++i) {
    int j = numTilesLinear;
    float x0 = (i + 0.5) * width0;
    float y0 = (j + 0.5) * height0;
    // World coordinates have origin at center.  So shift by targetSize / 2
    x0 -= 0.5f * targetImage.width();
    y0 -= 0.5f * targetImage.height();
    for (auto t = mosaic.cTilesBegin(); t != mosaic.cTilesEnd(); ++ t) {
      badness +=
        computeBadnessPair(x0, y0, width0, height0, 0.0f, 1.0f,
            t->x_, t->y_, t->image_->width(), t->image_->height(),
            t->angle_, t->scale_,
            potential_.get());
    }
  }

  // left column of fake tiles
  for (int j = 0; j != numTilesLinear; ++j) {
    int i = -1;
    float x0 = (i + 0.5) * width0;
    float y0 = (j + 0.5) * height0;
    // World coordinates have origin at center.  So shift by targetSize / 2
    x0 -= 0.5f * targetImage.width();
    y0 -= 0.5f * targetImage.height();
    for (auto t = mosaic.cTilesBegin(); t != mosaic.cTilesEnd(); ++ t) {
      badness +=
        computeBadnessPair(x0, y0, width0, height0, 0.0f, 1.0f,
            t->x_, t->y_, t->image_->width(), t->image_->height(),
            t->angle_, t->scale_,
            potential_.get());
    }
  }

  return badness;
}

void InteractionTileBorder::resetPotential(
    std::unique_ptr<Potential> potential) {
  potential_ = std::move(potential);
}

}
