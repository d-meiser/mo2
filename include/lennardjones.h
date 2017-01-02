#ifndef LENNARDJONES_H
#define LENNARDJONES_H

#include <mo_lib_export.h>


namespace Mo {

class MO_LIB_EXPORT LennardJones {
public:
    LennardJones(float epsilon, float sigma);
    float evaluateAt(float r) const;
    float epsilon() const {
        return epsilon_;
    }
    float sigma() const {
        return sigma_;
    }
private:
    float epsilon_;
    float sigma_;
};

}

#endif // LENNARDJONES_H
