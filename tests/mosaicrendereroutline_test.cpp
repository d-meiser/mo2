#include <gtest/gtest.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <image.h>
#include <test_utilities.h>
#include <mosaic_renderer_outline.h>
#include <mosaic.h>
#include <image.h>
#include <tile.h>


static GLFWwindow* window;

static void key_callback(GLFWwindow* window, int key, int, int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


struct MosaicRendererOutline : public ::testing::Test {
  MosaicRendererOutline() :
    mosaic(Mo::Image(150, 100), 1.4f)
  {}
  virtual void SetUp() {
    createSomeModel(2);
  }
  virtual void TearDown() {}
  void createSomeModel(int numTiles) {
    std::vector<Mo::Tile> tiles;
    for (int i = 0; i != numTiles; ++i) {
      tiles.emplace_back(
          Mo::Tile{0, 0, 0, 1.0f, std::unique_ptr<Mo::Image>(
              new Mo::Image(30, 20))});
    }
    mosaic.addTiles(&tiles[0], &tiles[0] + tiles.size());
  }
  Mo::Mosaic mosaic;
};

TEST_F(MosaicRendererOutline, IncludeTest) {
  EXPECT_TRUE(true);
}

TEST_F(MosaicRendererOutline, ConstructorTest) {
  Mo::MosaicRendererOutline renderer;
}

TEST_F(MosaicRendererOutline, CanRender) {
  Mo::MosaicRendererOutline renderer;
  renderer.setMosaic(&mosaic);
  int width;
  int height;
  for (int i = 0; i < 2; ++i) {
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    renderer.render();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  Mo::Image image(width, height);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE,
      image.getPixelData());

  image.save("outline.jpg");
  //Mo::Image referenceImage(testFile("passThroughRenderer_master.jpg"));
  //EXPECT_EQ(referenceImage, image);
}

int main(int argn, char* argv[]) {
  ::testing::InitGoogleTest(&argn, argv);

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, false);
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);

  gl3wInit();

  int result = RUN_ALL_TESTS();

  glfwDestroyWindow(window);
  glfwTerminate();
  return result;
}

