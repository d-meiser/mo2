#ifndef POTENTIALLENNARDJONES_H
#define POTENTIALLENNARDJONES_H

#include <lennardjones.h>
#include <potential.h>
#include <mo_lib_export.h>


namespace Mo {

class MO_LIB_EXPORT PotentialLennardJones : public Potential {
public:
    PotentialLennardJones(const LennardJones& lennardJones) :
        lennardJones_(lennardJones) {
    }

    virtual ~PotentialLennardJones() {}
    virtual float operator()(const float* x1, const float* x2);
    virtual float range() const;
private:
    LennardJones lennardJones_;
};

}

#endif // POTENTIALLENNARDJONES_H
