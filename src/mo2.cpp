#include <vector>
#include <string>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <mo2.h>

using namespace Mo;

namespace {

GLFWwindow* window;
static const int width = 640;
static const int height = 480;

void initOpenGL() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, true);
  window = glfwCreateWindow(width, height, "Simple example", NULL, NULL);
  glfwMakeContextCurrent(window);

  gl3wInit();
}

void terminateOpenGL() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

}


int main(int argn, const char *argv[]) {
  initOpenGL();
  RunConfiguration config(argn, argv);

  Mosaic mosaic{
      *config.targetImageName(),
      1.0f,
      config.tileBegin(),
      config.tileEnd()};

  Mo::MosaicRendererTextured renderer;
  renderer.setMosaic(&mosaic);
  renderer.setTileImages(mosaic.getTiles());

  glViewport(0, 0, width, height);
  for (int i = 0; ; ++i) {
    renderer.render();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  terminateOpenGL();

  return 0;
}


