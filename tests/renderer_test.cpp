#include <gtest/gtest.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <renderer.h>
#include <image.h>
#include <test_utilities.h>

using Mo::Testing::testFile;


static GLFWwindow* window;

static void key_callback(GLFWwindow* window, int key, int, int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


TEST(renderer, IncludeTest) { }

class PassThroughRenderer : public Mo::Renderer {
public:
  PassThroughRenderer() : framecount_(0) {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
  }

  ~PassThroughRenderer() {
    glDeleteVertexArrays(1, &vao_);
  }

private:
  const char* vertexShaderSource() override {
    return
    "#version 150\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),\n"
    "                                   vec4(-0.25, -0.25, 0.5, 1.0),\n"
    "                                   vec4(0.25, 0.25, 0.5, 1.0));\n"
    "  gl_Position = vertices[gl_VertexID];\n"
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

  void bindVAO() override {
    glBindVertexArray(vao_);
  }

  void draw() override {
    const GLfloat color[] = {0.0f, 0.2f, 0.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, color);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    ++framecount_;
  }

  GLuint vao_;
  int framecount_;
};

TEST(renderer, CanConstruct) {
  PassThroughRenderer renderer;
}

TEST(renderer, CanRender) {
  PassThroughRenderer renderer;
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

  Mo::Image referenceImage(testFile("passThroughRenderer_master.jpg"));
  EXPECT_EQ(referenceImage, image);
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


