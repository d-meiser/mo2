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
  if (std::abs(x) < n) {
    return Sinc(x / n) * Sinc(x);
  } else {
    return 0.0f;
  }
}

}

