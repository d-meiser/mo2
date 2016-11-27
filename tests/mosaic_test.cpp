#include <gtest/gtest.h>
#include <mosaic.h>
#include <test_utilities.h>

using Mo::Testing::testFile;


TEST(Mosaic, ConstructorTest) {
  Mo::Mosaic mosaic(testFile("test_image_1.jpg"), nullptr, nullptr);
}

