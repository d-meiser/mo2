#include <gtest/gtest.h>
#include <image.h>
#include <test_utilities.h>

using Mo::Testing::testFile;


TEST(Image, CanBeConstructedFromFileName) {
  Mo::Image image(testFile("test_image_1.jpg"));
}

TEST(Image, CanBeConstructedFromDimensions) {
  Mo::Image image(50, 30);
}

TEST(Image, HasCorrectWidth) {
  Mo::Image image(testFile("test_image_1.jpg"));
  EXPECT_EQ(30, image.width());
}

TEST(Image, ColorImagesHaveThreeOrFourComponents) {
  Mo::Image image(testFile("test_image_1.jpg"));
  EXPECT_TRUE(4 == image.numComponents() || 3 == image.numComponents());
}

TEST(Image, PitchIsBoundedFromBelowByImageWidth) {
  Mo::Image image(testFile("test_image_1.jpg"));
  EXPECT_GE(image.pitch(), image.numComponents() * image.width());
}

TEST(Image, CanSaveImage) {
  Mo::Image image(testFile("test_image_1.jpg"));
  EXPECT_NO_THROW(image.save("saved_file.jpg"));
}

TEST(Image, CanSetQuality) {
  Mo::Image image(testFile("test_image_1.jpg"));
  image.setQuality(30);
  EXPECT_NO_THROW(image.save("saved_file_2.jpg"));
}

