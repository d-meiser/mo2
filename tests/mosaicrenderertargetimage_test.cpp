#include <gtest/gtest.h>
#include <mosaic_renderer_targetimage.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <image.h>
#include <mosaic.h>
#include <framebuffer.h>


namespace {

static const int width = 160;
static const int height = 120;

static GLFWwindow* window;

static void key_callback(GLFWwindow* window, int key, int, int action, int) {
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

struct MosaicRendererTargetImage : public ::testing::Test {
  MosaicRendererTargetImage() :
    mosaic(Mo::Image(width, height), 1.4f)
  {
    Mo::Image img{mosaic.targetImage().image()};
    fillImage(0, &img);
    mosaic.setTargetImage(Mo::TargetImage{img, mosaic.targetImage().scale()});
  }

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
    }
    mosaic.addTiles(&tiles[0], &tiles[0] + tiles.size());
  }
  Mo::Mosaic mosaic;
};

TEST_F(MosaicRendererTargetImage, Constructor) {
  Mo::MosaicRendererTargetImage renderer(mosaic.targetImage().width(),
      mosaic.targetImage().height());
}

TEST_F(MosaicRendererTargetImage, RendersTargetImage) {
  Mo::MosaicRendererTargetImage renderer(
      mosaic.targetImage().width(),
      mosaic.targetImage().height());
  mosaic.targetImage().image().save("originalTargetImage.jpg");
  renderer.setMosaic(mosaic);
  renderer.setTileImages(mosaic.getTiles());
  Mo::Framebuffer framebuffer(static_cast<int>(mosaic.targetImage().width()),
                              static_cast<int>(mosaic.targetImage().height()));
  framebuffer.bind();
  renderer.render();
  Mo::Image renderedMosaic(framebuffer.width(), framebuffer.height(), 4);
  framebuffer.getPixels(renderedMosaic.getPixelData());
  renderedMosaic.save("renderedTargetImage.jpg");
}

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

