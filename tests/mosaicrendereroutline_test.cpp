#include <gtest/gtest.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <image.h>
#include <test_utilities.h>
#include <mosaic_renderer_outline.h>
#include <mosaic.h>
#include <image.h>
#include <tile.h>

using Mo::Testing::testFile;


static const int width = 160;
static const int height = 120;

static GLFWwindow* window;

static void key_callback(GLFWwindow* window, int key, int, int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


struct MosaicRendererOutline : public ::testing::Test {
  MosaicRendererOutline() :
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
    }
    mosaic.addTiles(&tiles[0], &tiles[0] + tiles.size());
  }
  Mo::Mosaic mosaic;
};

TEST_F(MosaicRendererOutline, IncludeTest) {
  EXPECT_TRUE(true);
}

TEST_F(MosaicRendererOutline, ConstructorTest) {
  Mo::MosaicRendererOutline renderer(width, height);
}

TEST_F(MosaicRendererOutline, CanRender) {
  Mo::MosaicRendererOutline renderer(width, height);
  renderer.setMosaic(mosaic);

  float red = 0.1f;
  float green = 0.2f;
  float blue = 1.0f;
  glClearColor(red, green, blue, 1.0);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < 2; ++i) {
    glViewport(0, 0, width, height);
    renderer.render();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  Mo::Image image(width, height);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE,
      image.getPixelData());

  Mo::Image master(testFile("mosaicOutlineRenderer_master.jpg"));
  EXPECT_LT(master.distance(image), 7.0e-2f);
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

