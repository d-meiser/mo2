#include <gtest/gtest.h>
#include <image.h>
#include <test_utilities.h>
#include <cmath>

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

void fillImageWithRandomBytes(int width, int height, unsigned char* pixels) {
  static const int numComponents = 3;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      for (int k = 0; k < numComponents; ++k) {
        pixels[j * numComponents + k] =
          static_cast<unsigned int>(rand() & 0xFF);
      }
    }
    pixels += width * numComponents;
  }
}

void fillImageWithSignWave(int width, int height,
    float kx, float ky, float kz, unsigned char* pixels) {
  static const int numComponents = 3;
  static const float amplitude = 255.0f;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      pixels[j * numComponents + 0] = amplitude * std::sin(j * kx);
      pixels[j * numComponents + 1] = amplitude * std::sin(j * ky);
      pixels[j * numComponents + 2] = amplitude * std::sin(j * kz);
    }
    pixels += width * numComponents;
  }
}

TEST(ImageScaling, StretchingByFactorOfOneIsIdentity) {
  int width = 160;
  int height = 80;
  Mo::Image original(width, height);
  fillImageWithRandomBytes(width, height, original.getPixelData());
  Mo::Image rescaled(width, height);
  original.stretch(width, height, rescaled.getPixelData());
  EXPECT_EQ(original, rescaled);
}

TEST(ImageScaling, UpSamples1DBandLimitedSignal) {
  int width = 32;
  int height = 8;
  Mo::Image original(width, height);
  float kx = 0.1f;
  float ky = 0.1f;
  float kz = 0.1f;
  fillImageWithSignWave(width, height, kx, ky, kz,
      original.getPixelData());
  int newWidth = 1.3 * width;
  Mo::Image rescaled(newWidth, height);
  original.stretch(newWidth, height, rescaled.getPixelData());
  float dx = (width - 1.0f) / (newWidth - 1.0f);
  int j = 10;
  float x = j * dx;
  const unsigned char* pixels = rescaled.getConstPixelData();
  EXPECT_NEAR(255.0f * std::sin(kx * x), pixels[j * 3], 255.0f * 1.0e-2f);
}

TEST(ImageScaling, DownSamples1DBandLimitedSignal) {
  int width = 32;
  int height = 8;
  Mo::Image original(width, height);
  float kx = 0.1f;
  float ky = 0.1f;
  float kz = 0.1f;
  fillImageWithSignWave(width, height, kx, ky, kz,
      original.getPixelData());
  int newWidth = 0.6 * width;
  Mo::Image rescaled(newWidth, height);
  original.stretch(newWidth, height, rescaled.getPixelData());
  float dx = (width - 1.0f) / (newWidth - 1.0f);
  int j = 10;
  float x = j * dx;
  const unsigned char* pixels = rescaled.getConstPixelData();
  EXPECT_NEAR(255.0f * std::sin(kx * x), pixels[j * 3], 255.0f * 1.0e-2f);
}

TEST(ImageScaling, ReproducesSinglePixel) {
  int width = 1;
  int height = 1;
  Mo::Image original(width, height);
  unsigned char* pixel = original.getPixelData();
  pixel[0] = 100;
  pixel[1] = 31;
  pixel[2] = 90;
  Mo::Image rescaled(width, height);
  original.stretch(width, height, rescaled.getPixelData());
  const unsigned char* newPixel = rescaled.getConstPixelData();
  EXPECT_EQ(pixel[0], newPixel[0]);
  EXPECT_EQ(pixel[1], newPixel[1]);
  EXPECT_EQ(pixel[2], newPixel[2]);
}

TEST(ImageScaling, StretchesAPixelIntoRow) {
  int width = 1;
  int height = 1;
  Mo::Image original(width, height);
  unsigned char* pixel = original.getPixelData();
  pixel[0] = 100;
  pixel[1] = 31;
  pixel[2] = 90;
  int newWidth = 10;
  Mo::Image rescaled(newWidth, height);
  original.stretch(newWidth, height, rescaled.getPixelData());
  const unsigned char* newPixel = rescaled.getConstPixelData();
  int i = 0;
  EXPECT_NEAR(pixel[0], newPixel[i * 3 + 0], 2);
  EXPECT_NEAR(pixel[1], newPixel[i * 3 + 1], 2);
  EXPECT_NEAR(pixel[2], newPixel[i * 3 + 2], 2);
  i = 2;
  EXPECT_NEAR(pixel[0], newPixel[i * 3 + 0], 2);
  EXPECT_NEAR(pixel[1], newPixel[i * 3 + 1], 2);
  EXPECT_NEAR(pixel[2], newPixel[i * 3 + 2], 2);
  i = 4;
  EXPECT_NEAR(pixel[0], newPixel[i * 3 + 0], 2);
  EXPECT_NEAR(pixel[1], newPixel[i * 3 + 1], 2);
  EXPECT_NEAR(pixel[2], newPixel[i * 3 + 2], 2);
}

TEST(ImageScaling, StretchesAPixelIntoColumn) {
  int width = 1;
  int height = 1;
  Mo::Image original(width, height);
  unsigned char* pixel = original.getPixelData();
  pixel[0] = 100;
  pixel[1] = 31;
  pixel[2] = 90;
  int newHeight = 10;
  Mo::Image rescaled(width, newHeight);
  original.stretch(width, newHeight, rescaled.getPixelData());
  const unsigned char* newPixel = rescaled.getConstPixelData();
  int i = 0;
  EXPECT_NEAR(pixel[0], newPixel[i * 3 * width + 0 + 0], 2);
  EXPECT_NEAR(pixel[1], newPixel[i * 3 * width + 0 + 1], 2);
  EXPECT_NEAR(pixel[2], newPixel[i * 3 * width + 0 + 2], 2);
  i = 2;
  EXPECT_NEAR(pixel[0], newPixel[i * 3 * width + 0 + 0], 2);
  EXPECT_NEAR(pixel[1], newPixel[i * 3 * width + 0 + 1], 2);
  EXPECT_NEAR(pixel[2], newPixel[i * 3 * width + 0 + 2], 2);
  i = 4;
  EXPECT_NEAR(pixel[0], newPixel[i * 3 * width + 0 + 0], 2);
  EXPECT_NEAR(pixel[1], newPixel[i * 3 * width + 0 + 1], 2);
  EXPECT_NEAR(pixel[2], newPixel[i * 3 * width + 0 + 2], 2);
}

TEST(ImageScaling, StretchesAPixelIntoRectangle) {
  int width = 1;
  int height = 1;
  Mo::Image original(width, height);
  unsigned char* pixel = original.getPixelData();
  pixel[0] = 100;
  pixel[1] = 31;
  pixel[2] = 90;
  int newWidth = 20;
  int newHeight = 10;
  Mo::Image rescaled(newWidth, newHeight);
  original.stretch(newWidth, newHeight, rescaled.getPixelData());
  const unsigned char* newPixel = rescaled.getConstPixelData();
  int i = 0;
  int j = 0;
  EXPECT_NEAR(pixel[0], newPixel[(i * newWidth + j) * 3 + 0], 2);
  EXPECT_NEAR(pixel[1], newPixel[(i * newWidth + j) * 3 + 1], 2);
  EXPECT_NEAR(pixel[2], newPixel[(i * newWidth + j) * 3 + 2], 2);
  i = 2;
  EXPECT_NEAR(pixel[0], newPixel[(i * newWidth + j) * 3 + 0], 2);
  EXPECT_NEAR(pixel[1], newPixel[(i * newWidth + j) * 3 + 1], 2);
  EXPECT_NEAR(pixel[2], newPixel[(i * newWidth + j) * 3 + 2], 2);
  i = 4;
  EXPECT_NEAR(pixel[0], newPixel[(i * newWidth + j) * 3 + 0], 2);
  EXPECT_NEAR(pixel[1], newPixel[(i * newWidth + j) * 3 + 1], 2);
  EXPECT_NEAR(pixel[2], newPixel[(i * newWidth + j) * 3 + 2], 2);
  i = 2;
  j = 5;
  EXPECT_NEAR(pixel[0], newPixel[(i * newWidth + j) * 3 + 0], 2);
  EXPECT_NEAR(pixel[1], newPixel[(i * newWidth + j) * 3 + 1], 2);
  EXPECT_NEAR(pixel[2], newPixel[(i * newWidth + j) * 3 + 2], 2);
  i = 4;
  j = 8;
  EXPECT_NEAR(pixel[0], newPixel[(i * newWidth + j) * 3 + 0], 2);
  EXPECT_NEAR(pixel[1], newPixel[(i * newWidth + j) * 3 + 1], 2);
  EXPECT_NEAR(pixel[2], newPixel[(i * newWidth + j) * 3 + 2], 2);
}

