#include <gtest/gtest.h>
#include <lanczos.h>
#include <cmath>


TEST(Sinc, IsOneAtZero) {
  EXPECT_NEAR(1.0f, Mo::Sinc(0.0f), 1.0e-6f);
}

TEST(Sinc, IsZeroAtOne) {
  EXPECT_NEAR(0.0f, Mo::Sinc(1.0f), 1.0e-6f);
}

TEST(Sinc, IsContinuous) {
  static const float smallX = 1.0e-3f;
  static const float epsilon = 1.0e-6f;
  EXPECT_NEAR(Mo::Sinc(smallX - epsilon),
              Mo::Sinc(smallX + epsilon),
              1.0e-6f);
}

TEST(Lanczos, IsOneAtZero) {
  EXPECT_NEAR(1.0f, Mo::Lanczos(2, 0.0f), 1.0e-6f);
}

TEST(Lanczos, IsZeroAtNonZeroIntegers) {
  EXPECT_NEAR(0.0f, Mo::Lanczos(2, 1.0f), 1.0e-6f);
  EXPECT_NEAR(0.0f, Mo::Lanczos(2, 2.0f), 1.0e-6f);
  EXPECT_NEAR(0.0f, Mo::Lanczos(2, 3.0f), 1.0e-6f);
  EXPECT_NEAR(0.0f, Mo::Lanczos(2, -1.0f), 1.0e-6f);
}

TEST(Lanczos, HasCompactSupport) {
  for (int i = 0; i < 10; ++i) {
    float x = 3.1f + i * 0.01f;
  EXPECT_NEAR(0.0f, Mo::Lanczos(3, x), 1.0e-6f);
  }
}

static double massOfLanczosKernel(double x) {
  static const int a = 3;
  x = x - std::floor(x) - a + 1;
  double sum = 0.0;
  for (int i = 0; i < 6; ++i) {
    sum += Mo::Lanczos(a, x);
    x += 1.0;
  }
  return sum;
}

TEST(Lanczos, SumsToApproximatelyOne) {
  double x = 0.0;
  int numSamples = 23;
  for (int i = 0; i < numSamples; ++i) {
    EXPECT_NEAR(1.0, massOfLanczosKernel(x), 0.1) <<
      "x == " << x <<
      "massOfLanczosKernel(x) == " << massOfLanczosKernel(x);
    x += 1.0 / numSamples;
  }
}
