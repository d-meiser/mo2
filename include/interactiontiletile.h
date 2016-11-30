#ifndef INTERACTIONTILETILE_H
#define INTERACTIONTILETILE_H

#include <badness.h>
#include <memory>

#include <mo_lib_export.h>


namespace Mo {

class Potential;


class MO_LIB_EXPORT InteractionTileTile : public Badness {
public:
    InteractionTileTile(std::unique_ptr<Potential> potential);
    virtual float computeBadness(const Mosaic &model,
        const TargetImage &targetImage);
    void resetPotential(std::unique_ptr<Potential> potential);
private:
    std::unique_ptr<Potential> potential_;
};

}

#endif // INTERACTIONTILETILE_H
