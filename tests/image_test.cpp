#include <gtest/gtest.h>
#include <image.h>
#include <test_utilities.h>

using Mo::Testing::testFile;


TEST(Image, CanBeConstructedFromFileName) {
  Mo::Image image(testFile("test_image_1.jpg").c_str());
}

TEST(Image, HasCorrectWidth) {
  Mo::Image image(testFile("test_image_1.jpg").c_str());
  EXPECT_EQ(30, image.width());
}

