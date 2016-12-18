#include <vector>
#include <string>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>

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

  glfwSetKeyCallback(window, keyCallback);

  gl3wInit();
}

void terminateOpenGL() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int,
    int action, int) {
  if (action == GLFW_PRESS) {
    switch (key) {
      case(GLFW_KEY_ESCAPE):
      case(GLFW_KEY_Q):
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    }
  }
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

  auto start = std::chrono::system_clock::now();
  long frame = 0;
  while (!glfwWindowShouldClose(window)) {
    renderer.render();

    glfwSwapBuffers(window);
    glfwPollEvents();

    if ((frame & 0xFF) == 0) {
      int numFrames = 0xFF + 1;
      auto end = std::chrono::system_clock::now();
      auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      std::cout << "FPS: " << numFrames / (1.0e-3 * elapsed.count()) << std::endl;
      start = end;
    }
    ++frame;
  }

  terminateOpenGL();

  return 0;
}


