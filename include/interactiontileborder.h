#ifndef INTERACTIONTILEBORDER_H
#define INTERACTIONTILEBORDER_H

#include <memory>

#include <badness.h>
#include <mo_lib_export.h>


namespace Mo {

class Potential;


class MO_LIB_EXPORT InteractionTileBorder : public Badness {
public:
    InteractionTileBorder(std::unique_ptr<Potential> potential);
    virtual float computeBadness(const Mosaic &model);
    void resetPotential(std::unique_ptr<Potential> potential);

private:
    std::unique_ptr<Potential> potential_;
};

}

#endif // INTERACTIONTILEBORDER_H
