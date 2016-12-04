#include <gtest/gtest.h>
#include <GL/gl3w.h>
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

TEST(glfw, ContextIs32Core) {
  EXPECT_TRUE(gl3wIsSupported(3, 2));
}

TEST(glfw, CanCreateFBO) {
  GLuint fbo;
  glGenFramebuffers(1, &fbo);
  GLint err = glGetError();
  EXPECT_EQ(GL_NO_ERROR, err);
  glDeleteFramebuffers(1, &fbo);
}

TEST(glfw, CanBindFBO) {
  GLuint fbo;
  glGenFramebuffers(1, &fbo);
  GLint err = glGetError();
  ASSERT_EQ(GL_NO_ERROR, err);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glDeleteFramebuffers(1, &fbo);
}

int main(int argn, char* argv[]) {
  ::testing::InitGoogleTest(&argn, argv);
  glfwInit();
  GLFWwindow* window = createWindow();
  glfwMakeContextCurrent(window);
  gl3wInit();
  int result = RUN_ALL_TESTS();
  glfwDestroyWindow(window);
  glfwTerminate();
  return result;
}
