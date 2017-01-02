#include <gtest/gtest.h>
#include <initial_guess.h>
#include <mosaic.h>
#include <mosaic_match.h>
#include <mosaic_renderer.h>
#include <mosaic_renderer_textured.h>
#include <badnesscomposite.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <potentiallennardjones.h>
#include <interactiontileborder.h>
#include <interactiontiletile.h>

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


std::unique_ptr<Mo::Badness> createTileBorderInteraction() {
  std::unique_ptr<Mo::Potential> potential{
    new Mo::PotentialLennardJones{Mo::LennardJones{5.0, 1.0}}};
  return std::unique_ptr<Mo::Badness>{new
    Mo::InteractionTileBorder{std::move(potential)}};
}

std::unique_ptr<Mo::Badness> createTileTileInteraction() {
  std::unique_ptr<Mo::Potential> potential{
    new Mo::PotentialLennardJones{Mo::LennardJones{5.0, 1.0}}};
  return std::unique_ptr<Mo::Badness>{new
    Mo::InteractionTileTile{std::move(potential)}};
}

std::unique_ptr<Mo::Badness>
createMosaicMatch(std::shared_ptr<Mo::MosaicRenderer> renderer) {
  return std::unique_ptr<Mo::Badness>{new Mo::MosaicMatch{renderer}};
}


struct InitialGuessT : public ::testing::Test {

  InitialGuessT() :
    mosaic(Mo::Image(width, height), 1.4f),
    renderer{std::make_shared<Mo::MosaicRendererTextured>(width, height)}
  {}

  virtual void SetUp() {
    createSomeModel(30);

    renderer->setMosaic(mosaic);
    renderer->setTileImages(mosaic.getTiles());

    badness.addBadness(std::move(createTileBorderInteraction()));
    badness.addBadness(std::move(createTileTileInteraction()));
    badness.addBadness(std::move(createMosaicMatch(renderer)));
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
  Mo::BadnessComposite badness;
  std::shared_ptr<Mo::MosaicRenderer> renderer;
};


TEST_F(InitialGuessT, GuessHasOneTileIfGivenOneTile) {
  Mo::InitialGuess guesser;
  mosaic.reduceSize(1);
  Mo::Mosaic guess{guesser.findInitialGuess(&badness, mosaic)};
  EXPECT_EQ(mosaic.getTiles().size(), guess.getTiles().size());
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

