#include <lennardjones.h>
#include <cmath>

namespace Mo {

LennardJones::LennardJones(float epsilon, float sigma) :
    epsilon_(epsilon),
    sigma_(sigma)
{}

float LennardJones::evaluateAt(float r) const {
    static const float rmin = 1.0e-3f * sigma_;
    float sigmaOverR = sigma_ /
            std::sqrt(r * r + rmin * rmin);
    float xi = sigmaOverR * sigmaOverR;
    xi = xi * xi * xi;
    float lj = 4.0 * epsilon_ * (xi * xi - xi);
    return lj;
}

}
