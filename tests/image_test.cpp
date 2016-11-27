#include <gtest/gtest.h>
#include <image.h>
#include <test_utilities.h>

using Mo::Testing::testFile;


TEST(Image, CanBeConstructedFromFileName) {
  Mo::Image image(testFile("test_image_1.jpg").c_str());
}

TEST(Image, CanBeConstructedFromDimensions) {
  Mo::Image image(50, 30);
}

TEST(Image, HasCorrectWidth) {
  Mo::Image image(testFile("test_image_1.jpg").c_str());
  EXPECT_EQ(30, image.width());
}

TEST(Image, ColorImagesHaveThreeOrFourComponents) {
  Mo::Image image(testFile("test_image_1.jpg").c_str());
  EXPECT_TRUE(4 == image.numComponents() || 3 == image.numComponents());
}

TEST(Image, PitchIsBoundedFromBelowByImageWidth) {
  Mo::Image image(testFile("test_image_1.jpg").c_str());
  EXPECT_GE(image.pitch(), image.numComponents() * image.width());
}

TEST(Image, CanSaveImage) {
  Mo::Image image(testFile("test_image_1.jpg").c_str());
  EXPECT_NO_THROW(image.save("saved_file.jpg"));
}

TEST(Image, CanSetQuality) {
  Mo::Image image(testFile("test_image_1.jpg").c_str());
  image.setQuality(30);
  EXPECT_NO_THROW(image.save("saved_file_2.jpg"));
}

