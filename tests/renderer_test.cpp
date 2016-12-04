#include <gtest/gtest.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <renderer.h>


TEST(renderer, IncludeTest) { }

class PassThroughRenderer : public Mo::Renderer {
private:
  const char* vertexShaderSource() override {
    return
    "#version 150\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  gl_Position = vec4(0.0, 0.0, 0.5, 1.0);\n"
    "}\n"
    ;
  }
  const char* fragmentShaderSource() override {
    return
    "#version 150\n"
    "\n"
    "out vec4 color;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  color = vec4(0.0, 0.8, 1.0, 1.0);\n"
    "}\n"
    ;
  }
};

TEST(renderer, CanConstruct) {
  PassThroughRenderer renderer;
}

TEST(renderer, CanRender) {
  PassThroughRenderer renderer;
  renderer.render();
}

int main(int argn, char* argv[]) {
  ::testing::InitGoogleTest(&argn, argv);

  glfwInit();
  GLFWwindow* window;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  glfwMakeContextCurrent(window);

  gl3wInit();

  int result = RUN_ALL_TESTS();

  glfwDestroyWindow(window);
  glfwTerminate();
  return result;
}


