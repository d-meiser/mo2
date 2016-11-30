#include <interactionhelpers.h>
#include <quadraturerule.h>
#include <potential.h>


namespace Mo {

float computeBadnessPair(
    float x0, float y0, float w0, float h0, float alpha0, float scale0,
    float x1, float y1, float w1, float h1, float alpha1, float scale1,
    Potential* potential) {

  float range = potential->range();
  if (range > 0) {
    // Check if distance between tiles is larger than range.
    // To compute the distance between tiles we construct a
    // bounding circle around each tile and we compute the
    // distance between these circles.
    float distanceBetweenTileCenters = std::sqrt(
        (x0 - x1) * (x0 - x1) +
        (y0 - y1) * (y0 - y1));
    float radius0 = std::sqrt(w0 * w0 + h0 * h0);
    float radius1 = std::sqrt(w1 * w1 + h1 * h1);
    float distanceBetweenTiles =
      distanceBetweenTileCenters - radius0 - radius1;
    if (distanceBetweenTiles > range) {
      return 0.0f;
    }
  }

  // asm("# computeBadnessPair");
  static const int order = 4;
  static const float* weights =
    &QuadratureRule[order - 1][MO_QUADRATURE_WEIGHTS][0];
  static const float* nodes =
    &QuadratureRule[order - 1][MO_QUADRATURE_NODES][0];


  // asm("# compute nodes for i tile");
  float r1[order][order][2];
  for (int ix = 0; ix < order; ++ix) {
    for (int iy = 0; iy < order; ++iy) {
      r1[ix][iy][0] = 0.5f * nodes[ix];
      r1[ix][iy][1] = 0.5f * nodes[iy];
      transformToWorldCoordinates(x0, y0, w0, h0, alpha0, scale0,
          r1[ix][iy]);
    }
  }

  // asm("# compute nodes for j tile");
  float r2[order][order][2];
  for (int ix = 0; ix < order; ++ix) {
    for (int iy = 0; iy < order; ++iy) {
      r2[ix][iy][0] = 0.5f * nodes[ix];
      r2[ix][iy][1] = 0.5f * nodes[iy];
      transformToWorldCoordinates(x1, y1, w1, h1, alpha1, scale1,
          r2[ix][iy]);
    }
  }

  // Now compute badness by integrating over both tiles
  // asm("# quadrature loop");
  float badness = 0.0f;
  for (int ix = 0; ix < order; ++ix) {
    for (int iy = 0; iy < order; ++iy) {
      for (int jx = 0; jx < order; ++jx) {
        for (int jy = 0; jy < order; ++jy) {
          badness +=
            weights[ix] * weights[iy] *
            weights[jx] * weights[jy] *
            potential->operator()(r1[ix][iy], r2[jx][jy]);
        }
      }
    }
  }
  // Gauss quadrature weights are normalized for integration over [-1, 1].
  // We divide by 2^4 to make the integrals normalized over the tiles.
  return badness / 16.0f;
}

void transformToWorldCoordinates(
    float x, float y, float w, float h, float alpha, float scale,
    float* r) {
  float rtemp[2];
  float c = cos(alpha);
  float s = sin(alpha);

  r[0] *= w;
  r[1] *= h;

  rtemp[0] = scale * (c * r[0] - s * r[1]) + x;
  rtemp[1] = scale * (s * r[0] + c * r[1]) + y;

  for (int i = 0; i < 2; ++i) {
    r[i] = rtemp[i];
  }
}

}
