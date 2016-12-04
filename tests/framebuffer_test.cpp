#include <gtest/gtest.h>
#include <framebuffer.h>


TEST(framebuffer, IncludeTest) {
  EXPECT_TRUE(true);
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

