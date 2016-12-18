#ifndef MO2_H
#define MO2_H

#include <image.h>
#include <mosaic.h>
#include <run_configuration.h>
#include <tile.h>
#include <utilities.h>
#include <mosaic_renderer.h>
#include <mosaic_renderer_outline.h>
#include <mosaic_renderer_textured.h>

namespace {

void keyCallback(GLFWwindow* window, int key, int scancode,
    int action, int mods);
void initOpenGL();
void terminateOpenGL();

}

#endif

