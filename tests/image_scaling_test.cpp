#include <gtest/gtest.h>
#include <lanczos.h>

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
