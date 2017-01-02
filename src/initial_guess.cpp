#include <initial_guess.h>
#include <utilities.h>

namespace Mo {

Mosaic InitialGuess::findInitialGuess(Badness* badness, const Mosaic& mosaic) {
  MO_UNUSED(badness);
  return Mosaic{mosaic.targetImage().image(), 1.0f,
      &*mosaic.cTilesBegin(), &*mosaic.cTilesEnd()};
}

}

