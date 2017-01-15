#include <gtest/gtest.h>
#include <initial_guess.h>
#include <mosaic.h>
#include <mosaic_match.h>
#include <mosaic_renderer.h>
#include <mosaic_renderer_textured.h>
#include <mosaic_renderer_outline.h>
#include <badnesscomposite.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <potentiallennardjones.h>
#include <interactiontileborder.h>
#include <interactiontiletile.h>
#include <framebuffer.h>
#include <utilities.h>


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
    new Mo::PotentialLennardJones{Mo::LennardJones{5.0, 10.0}}};
  Mo::InteractionTileBorder* badness{new
    Mo::InteractionTileBorder{std::move(potential)}};
  badness->setBorder(0.8);
  return std::unique_ptr<Mo::Badness>{badness};
}

std::unique_ptr<Mo::Badness> createTileTileInteraction() {
  std::unique_ptr<Mo::Potential> potential{
    new Mo::PotentialLennardJones{Mo::LennardJones{5.0, 10.0}}};
  Mo::InteractionTileTile* badness{new
    Mo::InteractionTileTile{std::move(potential)}};
  badness->setBorder(0.8);
  return std::unique_ptr<Mo::Badness>{badness};
}

std::unique_ptr<Mo::Badness>
createMosaicMatch(std::shared_ptr<Mo::MosaicRenderer> renderer) {
  return std::unique_ptr<Mo::Badness>{new Mo::MosaicMatch{renderer}};
}


struct InitialGuessT : public ::testing::Test {

  InitialGuessT() :
    mosaic(Mo::Image(width, height), 1.4f),
    renderer{std::make_shared<Mo::MosaicRendererTextured>(width, height)},
    framebuffer{static_cast<int>(mosaic.targetImage().width()),
                static_cast<int>(mosaic.targetImage().height())}
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
  Mo::Framebuffer framebuffer;
};


TEST_F(InitialGuessT, GuessHasSameNumberOfTilesAsOriginal) {
  Mo::InitialGuess guesser;
  mosaic.reduceSize(6);
  Mo::Mosaic guess{guesser.findInitialGuess(&badness, mosaic)};
  EXPECT_EQ(mosaic.getTiles().size(), guess.getTiles().size());
}

TEST_F(InitialGuessT, GuessIsPlausibleForOneTile) {
  Mo::InitialGuess guesser;
  mosaic.reduceSize(1);
  Mo::Mosaic guess{guesser.findInitialGuess(&badness, mosaic)};

  renderer->setMosaic(guess);
  renderer->setTileImages(guess.getTiles());
  framebuffer.bind();
  renderer->render();
  Mo::dumpFramebuffer(framebuffer, "renderedMosaic.jpg");
}

TEST_F(InitialGuessT, GuessIsPlausibleForSeveralTiles) {
  Mo::InitialGuess guesser;
  mosaic.reduceSize(9);
  Mo::Mosaic guess{guesser.findInitialGuess(&badness, mosaic)};

  renderer->setMosaic(guess);
  renderer->setTileImages(guess.getTiles());
  framebuffer.bind();
  renderer->render();
  Mo::Image renderedMosaic(framebuffer.width(), framebuffer.height(), 4);
  framebuffer.getPixels(renderedMosaic.getPixelData());
  renderedMosaic.save("renderedMosaic10.jpg");

/*
  std::shared_ptr<Mo::MosaicRenderer>
      outlineRenderer{std::make_shared<Mo::MosaicRendererOutline>(
          framebuffer.width(),
          framebuffer.height())};
  auto t = guess.tilesEnd();
  t--;
  t->x_ = 0;
  t->y_ = 0;
  outlineRenderer->setMosaic(guess);
  outlineRenderer->setTileImages(guess.getTiles());
  framebuffer.bind();
  outlineRenderer->render();
  framebuffer.getPixels(renderedMosaic.getPixelData());
  renderedMosaic.save("renderedMosaicOutline.jpg");
*/
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

