#include <lanczos.h>
#include <cmath>

namespace Mo {

float Sinc(float x) {
  static const float smallX = 1.0e-3f;
  if (std::abs(x) < smallX) {
    return 1.0f - 0.5f * M_PI * x * x;
  } else {
    return sin(M_PI * x) / (M_PI * x);
  }
}

float Lanczos(int n, float x) {
  if (x < n) {
    return Sinc(n * x) * Sinc(x);
  } else {
    return 0.0f;
  }
}

}

