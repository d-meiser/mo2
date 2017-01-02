#ifndef POTENTIALLENNARDJONES_H
#define POTENTIALLENNARDJONES_H

#include <lennardjones.h>
#include <potential.h>

namespace Mo {

class PotentialLennardJones : public Potential {
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
