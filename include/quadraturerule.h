#ifndef QUADRATURE_RULE_H
#define QUADRATURE_RULE_H

#include <cmath>

#define MO_QUADRATURE_NODES 0
#define MO_QUADRATURE_WEIGHTS 1

namespace Mo {

// Nodes and weights for Gauss-Legendre quadrature (unit weighting function)
// on the interval [-1, 1].  Exact for polynomials of order 2n - 1.
static float QuadratureRule[4][2][4] = {
    {{0.0f, 0.0f, 0.0f, 0.0f},
     {2.0f, 0.0f, 0.0f, 0.0f}},
    {{-std::sqrt(1.0f / 3.0f), std::sqrt(1.0f / 3.0f), 0.0f, 0.0f},
     {1.0f, 1.0f, 0.0f, 0.0f}},
    {{-std::sqrt(3.0f / 5.0f), 0.0f, std::sqrt(3.0f / 5.0f), 0.0f},
     {5.0f / 9.0f, 8.0f / 9.0f, 5.0f / 9.0f, 0}},
    {{-std::sqrt(3.0f / 7.0f + 2.0f / 7.0f * std::sqrt(6.0f / 5.0f)),
      -std::sqrt(3.0f / 7.0f - 2.0f / 7.0f * std::sqrt(6.0f / 5.0f)),
      std::sqrt(3.0f / 7.0f - 2.0f / 7.0f * std::sqrt(6.0f / 5.0f)),
      std::sqrt(3.0f / 7.0f + 2.0f / 7.0f * std::sqrt(6.0f / 5.0f))},
     {(18.0f - std::sqrt(30.0f)) / 36.0f,
      (18 + std::sqrt(30.0f)) / 36.0f,
      (18 + std::sqrt(30.0f)) / 36.0f,
      (18.0f - std::sqrt(30.0f)) / 36.0f}}
};

}

#endif // QUADRATURE_H
