#include <gtest/gtest.h>
#include <framebuffer.h>


TEST(framebuffer, IncludeTest) {
  EXPECT_TRUE(true);
}

TEST(framebuffer, CanCreateFramebuffer) {
  Mo::Framebuffer fb(640, 480);
}

TEST(framebuffer, HasExpectedSize) {
  Mo::Framebuffer fb(640, 480);
  size_t expectedSize = 640 * 480 * 4;
  EXPECT_EQ(expectedSize, fb.size());
}

TEST(framebuffer, CanBindFB) {
  Mo::Framebuffer fb(640, 480);
  fb.bind();
}

TEST(framebuffer, CanGetPixels) {
  Mo::Framebuffer fb(640, 480);
  fb.bind();
  float red = 0.1f;
  float green = 0.2f;
  float blue = 1.0f;
  glClearColor(red, green, blue, 1.0);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GLint err = glGetError();
  ASSERT_EQ(GL_NO_ERROR, err);

  std::vector<unsigned char> pixelData(fb.size());
  fb.getPixels(&pixelData[0]);

  EXPECT_EQ((unsigned char)(red * 255), pixelData[0]);
  EXPECT_EQ((unsigned char)(green * 255), pixelData[1]);
  EXPECT_EQ((unsigned char)(blue * 255), pixelData[2]);
}

int main(int argn, char* argv[]) {
  ::testing::InitGoogleTest(&argn, argv);

  glfwInit();
  GLFWwindow* window;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, false);
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  glfwMakeContextCurrent(window);

  gl3wInit();

  int result = RUN_ALL_TESTS();

  glfwDestroyWindow(window);
  glfwTerminate();
  return result;
}

