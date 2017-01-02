#include <potentiallennardjones.h>

#include <cmath>


namespace Mo {

float PotentialLennardJones::operator()(const float* x1, const float* x2) {
    float r = std::sqrt(
                (x1[0] - x2[0]) * ( x1[0] - x2[0]) +
                (x1[1] - x2[1]) * ( x1[1] - x2[1]));
    return lennardJones_.evaluateAt(r);
}


float PotentialLennardJones::range() const {
    return 3.0f * lennardJones_.sigma();
}

}
