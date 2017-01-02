#include <gtest/gtest.h>
#include <mosaic_match.h>
#include <mosaic_renderer_textured.h>
#include <renderer.h>
#include <image.h>
#include <mosaic.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#ifndef M_PI
#define M_PI 3.14159265359
#endif


namespace {

static const int width = 160;
static const int height = 120;
static GLFWwindow* window;

static void key_callback(GLFWwindow* window, int key, int, int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void fillImage(int i, Mo::Image* img) {
  unsigned char* p = img->getPixelData();
  for (int m = 0; m < img->height(); ++m) {
    for (int n = 0; n < img->width(); ++n) {
      for (int k = 0; k < 3; ++k) {
        if (i % 3 == k) {
          p[(m * img->width() + n) * 3 + k] = (150 + m) & 0xFF;
        } else {
          p[(m * img->width() + n) * 3 + k] = (50 + n) & 0xFF;
        }
      }
    }
  }
}

}

struct MosaicRendererTextured : public ::testing::Test {
  MosaicRendererTextured() :
    mosaic(Mo::Image(width, height), 1.4f)
  {}
  virtual void SetUp() {
    createSomeModel(30);
  }
  virtual void TearDown() {}
  void createSomeModel(int numTiles) {
    std::vector<Mo::Tile> tiles;
    for (int i = 0; i != numTiles; ++i) {
      tiles.emplace_back(
          Mo::Tile{static_cast<float>(i * 1 - 30),
                   static_cast<float>(i * 2 - 30),
                   static_cast<float>(i * 0.1),
                   static_cast<float>(1.0f - 0.01 * i),
                   std::unique_ptr<Mo::Image>(
                       new Mo::Image(30 + i, 20 + 2 * i))});
      fillImage(i, tiles[i].image_.get());
    }
    mosaic.addTiles(&tiles[0], &tiles[0] + tiles.size());
  }
  Mo::Mosaic mosaic;
};


struct MosaicMatch_F : public MosaicRendererTextured {
  MosaicMatch_F() :
    renderer_{std::make_shared<Mo::MosaicRendererTextured>(width, height)}
    {}

  void SetUp() override {
    MosaicRendererTextured::SetUp();
    renderer_->setMosaic(mosaic);
    renderer_->setTileImages(mosaic.getTiles());
  }

  void TearDown() override {
    MosaicRendererTextured::TearDown();
  }

  std::shared_ptr<Mo::MosaicRenderer> renderer_;
};


////////////////////////////////////////////////
// Tests

TEST_F(MosaicMatch_F, Constructor) {
  Mo::MosaicMatch match{renderer_};
}

TEST_F(MosaicMatch_F, HasSmallBadnessForGoodMatch) {
  Mo::MosaicMatch match{renderer_};
  mosaic.reduceSize(1);
  auto& t = *mosaic.tilesBegin();
  t.x_ = 0.0f;
  t.y_ = 0.0f;
  t.angle_ = 0.0f;
  t.scale_ = 1.0f;
  mosaic.setTargetImage(Mo::TargetImage{*mosaic.cTilesBegin()->image_, 1.0f});
  EXPECT_LT(match.computeBadness(mosaic), 3.0e-2f);
}

TEST_F(MosaicMatch_F, BadnessIsLargerIfColorIsWrong) {
  Mo::MosaicMatch match{renderer_};
  mosaic.reduceSize(1);
  auto& t = *mosaic.tilesBegin();
  t.x_ = 0.0f;
  t.y_ = 0.0f;
  t.angle_ = 0.0f;
  t.scale_ = 1.0f;
  mosaic.setTargetImage(Mo::TargetImage{*mosaic.cTilesBegin()->image_, 1.0f});
  float smallBadness = match.computeBadness(mosaic);
  Mo::Image* img = mosaic.tilesBegin()->image_.get();
  unsigned char* pixels = img->getPixelData();
  int width = img->width();
  for (int i = 0; i < img->height(); ++i) {
    for (int j = 0; j < img->width(); ++j) {
      pixels[(i * width + j) * 3 + 0] = 0;
    }
  }
  float largerBadness = match.computeBadness(mosaic);
  EXPECT_GT(largerBadness, smallBadness);
}

TEST_F(MosaicMatch_F, OrientationMatters) {
  Mo::MosaicMatch match{renderer_};
  mosaic.reduceSize(1);
  auto& t = *mosaic.tilesBegin();
  t.x_ = 0.0f;
  t.y_ = 0.0f;
  t.angle_ = 0.0f;
  t.scale_ = 1.0f;

  // Create an image that is not rotationally symmetric.
  Mo::Image* img = mosaic.tilesBegin()->image_.get();
  unsigned char* pixels = img->getPixelData();
  int width = img->width();
  for (int i = 0; i < img->height(); ++i) {
    for (int j = img->width() / 2; j < img->width(); ++j) {
      pixels[(i * width + j) * 3 + 0] = 0;
    }
  }

  mosaic.setTargetImage(Mo::TargetImage{*mosaic.cTilesBegin()->image_, 1.0f});

  // First compute the badness with correct orientation.
  float smallBadness = match.computeBadness(mosaic);

  // Now compute badness with wrong orientation.
  t.angle_ = M_PI;
  float largerBadness = match.computeBadness(mosaic);

  std::cout << smallBadness << std::endl;
  std::cout << largerBadness << std::endl;

  EXPECT_GT(largerBadness, smallBadness);
}

TEST_F(MosaicMatch_F, WeirdAnglesAreOKToo) {
  Mo::MosaicMatch match{renderer_};
  mosaic.reduceSize(1);
  auto& t = *mosaic.tilesBegin();
  t.x_ = 0.0f;
  t.y_ = 0.0f;
  t.angle_ = 0.0f;
  t.scale_ = 1.0f;

  // Create an image that is not rotationally symmetric.
  Mo::Image* img = mosaic.tilesBegin()->image_.get();
  unsigned char* pixels = img->getPixelData();
  int width = img->width();
  for (int i = 0; i < img->height(); ++i) {
    for (int j = img->width() / 2; j < img->width(); ++j) {
      pixels[(i * width + j) * 3 + 0] = 0;
    }
  }

  mosaic.setTargetImage(Mo::TargetImage{*mosaic.cTilesBegin()->image_, 1.0f});
  mosaic.targetImage().image().save("targetImage.jpg");

  // First compute the badness with correct orientation.
  float smallBadness = match.computeBadness(mosaic);

  // Now compute badness with wrong orientation.
  t.angle_ = 0.97;
  float largerBadness = match.computeBadness(mosaic);

  std::cout << smallBadness << std::endl;
  std::cout << largerBadness << std::endl;

  EXPECT_GT(largerBadness, smallBadness);
}


int main(int argn, char* argv[]) {
  ::testing::InitGoogleTest(&argn, argv);

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, false);
  window = glfwCreateWindow(width, height, "Simple example", NULL, NULL);
  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);

  gl3wInit();

  int result = RUN_ALL_TESTS();

  glfwDestroyWindow(window);
  glfwTerminate();

  return result;
}

