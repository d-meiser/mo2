#ifndef INITIAL_GUESS_H
#define INITIAL_GUESS_H

#include <mosaic.h>
#include <badness.h>
#include <mo_lib_export.h>


namespace Mo {

class MO_LIB_EXPORT InitialGuess {
public:
  Mosaic findInitialGuess(Badness* badness, const Mosaic& mosaic);
};

}

#endif

