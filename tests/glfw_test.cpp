#include <gtest/gtest.h>
#include <GLFW/glfw3.h>


TEST(glfw, CanCreateOffscreenContext) {
  GLFWwindow* window;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_VISIBLE, false);
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  EXPECT_NE(nullptr, window);
  glfwDestroyWindow(window);
}

int main(int argn, char* argv[]) {
  ::testing::InitGoogleTest(&argn, argv);
  glfwInit();
  int result = RUN_ALL_TESTS();
  glfwTerminate();
  return result;
}
