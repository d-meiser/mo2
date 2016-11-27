#include <gtest/gtest.h>
#include <image.h>

TEST(Image, CanBeConstructedFromFileName) {
  Mo::Image image("test_image.jpg");
}

