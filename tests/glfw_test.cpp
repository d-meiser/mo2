#include <gtest/gtest.h>
#include <GLFW/glfw3.h>


static GLFWwindow* createWindow() {
  GLFWwindow* window;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, false);
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  return window;
}

TEST(glfw, CanCreateOffscreenContext) {
  GLFWwindow* window = createWindow();
  EXPECT_NE(nullptr, window);
  glfwDestroyWindow(window);
}

TEST(glfw, CanCreateFBO) {
  GLFWwindow* window = createWindow();
  ASSERT_NE(nullptr, window);
  glfwDestroyWindow(window);
}

int main(int argn, char* argv[]) {
  ::testing::InitGoogleTest(&argn, argv);
  glfwInit();
  int result = RUN_ALL_TESTS();
  glfwTerminate();
  return result;
}
