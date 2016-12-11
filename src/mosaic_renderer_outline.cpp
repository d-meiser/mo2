#include <mosaic_renderer_outline.h>
#include <tile.h>
#include <utilities.h>
#include <mosaic.h>
#include <GLFW/glfw3.h>
#include <algorithm>


namespace Mo {

static const char vShaderSource[] =
    "#version 150\n"
    "\n"
    "in float x;\n"
    "in float y;\n"
    "in float width;\n"
    "in float height;\n"
    "in float rotation;\n"
    "\n"
    "uniform float viewPortWidth = 1000.0f;\n"
    "uniform float viewPortHeight = 1000.0f;\n"
    "uniform float magnification = 1.0f;\n"
    "uniform float numTiles = 10.0f;\n"
    "\n"
    "const vec2 pos[] = vec2[4](\n"
    "  vec2(-0.5f,  0.5f),\n"
    "  vec2(-0.5f, -0.5f),\n"
    "  vec2( 0.5f,  0.5f),\n"
    "  vec2( 0.5f, -0.5f)\n"
    ");\n"
    "\n"
    "#define NUM_COLORS 3\n"
    "const vec4 colors[] = vec4[3](\n"
    "  vec4(1.0, 0.0, 0.0, 1.0),\n"
    "  vec4(0.0, 1.0, 0.0, 1.0),\n"
    "  vec4(0.0, 0.0, 1.0, 1.0)\n"
    ");\n"
    "\n"
    "out vec4 color;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    vec2 offset_ = pos[gl_VertexID];\n"
    "    offset_.x *= width;\n"
    "    offset_.y *= height;\n"
    "    float c_ = cos(rotation);\n"
    "    float s_ = sin(rotation);\n"
    "    float x_ = (x + c_ * offset_.x - s_ * offset_.y);\n"
    "    float y_ = (y + s_ * offset_.x + c_ * offset_.y);\n"
    "    gl_Position = vec4(2.0f * magnification * x_ / viewPortWidth,\n"
    "                       2.0f * magnification * y_ / viewPortHeight,\n"
    "                       (gl_InstanceID - numTiles) / numTiles,\n"
    "                       1.0f);\n"
    "    color = colors[gl_InstanceID % NUM_COLORS];\n"
    "}\n"
    ;

static const char fShaderSource[] =
    "#version 330\n"
    "\n"
    "in vec4 color;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  gl_FragColor = color;\n"
    "}\n"
    ;

MosaicRendererOutline::MosaicRendererOutline() :
    vbo_(0),
    vao_(0),
    glBuffersUpToDate_(false),
    viewPortWidth_(-1),
    viewPortHeight_(-1),
    magnification_(-1),
    numTiles_(-1) {
}

MosaicRendererOutline::~MosaicRendererOutline() {
  if (vbo_) {
    glDeleteBuffers(1, &vbo_);
  }
  if (vao_) {
    glDeleteVertexArrays(1, &vao_);
  }
}

void MosaicRendererOutline::setMosaic(Mosaic* mosaic) {
  tiles_.resize(mosaic->size());
  std::transform(mosaic->cTilesBegin(), mosaic->cTilesEnd(),
      tiles_.begin(),
      [](const Tile& t) {
        return MyTile{t.x_, t.y_, t.angle_, t.width(), t.height()};
      });
  glBuffersUpToDate_ = false;
}

void MosaicRendererOutline::setTileImages(const std::vector<Tile>& tiles) {
  MO_UNUSED(tiles);
}

const char* MosaicRendererOutline::vertexShaderSource() {
  return vShaderSource;
}

const char* MosaicRendererOutline::fragmentShaderSource() {
  return fShaderSource;
}

void MosaicRendererOutline::bindVAO() {
  MO_CHECK_GL_ERROR;
  setupVAO();
  glBindVertexArray(vao_);
  MO_CHECK_GL_ERROR;
}

void MosaicRendererOutline::draw() {
  MO_CHECK_GL_ERROR;
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CW);
  glCullFace(GL_FRONT);
  MO_CHECK_GL_ERROR;

  const GLfloat color[] = {0.2f, 0.2f, 0.2f, 1.0f};
  glClearBufferfv(GL_COLOR, 0, color);
  const float one = 1.0f;
  glClearBufferfv(GL_DEPTH, 0, &one);
  MO_CHECK_GL_ERROR;


  if (viewPortWidth_ < 0) {
    getUniformLocations();
  }
  int width;
  int height;
  GLFWwindow* window = glfwGetCurrentContext();
  glfwGetFramebufferSize(window, &width, &height);
  glUniform1f(viewPortWidth_, static_cast<float>(width));
  glUniform1f(viewPortHeight_, static_cast<float>(height));
  glUniform1f(magnification_, 1.0f);
  glUniform1f(numTiles_, static_cast<float>(tiles_.size()));
  MO_CHECK_GL_ERROR;

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, tiles_.size());
  MO_CHECK_GL_ERROR;
}

void MosaicRendererOutline::setupVAO() {
  MO_CHECK_GL_ERROR;

  if (!vao_) {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    MO_CHECK_GL_ERROR;

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    MO_CHECK_GL_ERROR;

    GLint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);
    MO_CHECK_GL_ERROR;

    glEnableVertexAttribArray(glGetAttribLocation(program, "x"));
    glEnableVertexAttribArray(glGetAttribLocation(program, "y"));
    glEnableVertexAttribArray(glGetAttribLocation(program, "width"));
    glEnableVertexAttribArray(glGetAttribLocation(program, "height"));
    glEnableVertexAttribArray(glGetAttribLocation(program, "rotation"));
    MO_CHECK_GL_ERROR;

    glVertexAttribPointer(glGetAttribLocation(program, "x"),
        1, GL_FLOAT, false, sizeof(MyTile),
        reinterpret_cast<void*>(0 * sizeof(float)));
    glVertexAttribPointer(glGetAttribLocation(program, "y"),
        1, GL_FLOAT, false, sizeof(MyTile),
        reinterpret_cast<void*>(1 * sizeof(float)));
    glVertexAttribPointer(glGetAttribLocation(program, "width"),
        1, GL_FLOAT, false, sizeof(MyTile),
        reinterpret_cast<void*>(3 * sizeof(float)));
    glVertexAttribPointer(glGetAttribLocation(program, "height"),
        1, GL_FLOAT, false, sizeof(MyTile),
        reinterpret_cast<void*>(4 * sizeof(float)));
    glVertexAttribPointer(glGetAttribLocation(program, "rotation"),
        1, GL_FLOAT, false, sizeof(MyTile),
        reinterpret_cast<void*>(2 * sizeof(float)));
    MO_CHECK_GL_ERROR;

    glVertexAttribDivisor(glGetAttribLocation(program, "x"), 1);
    glVertexAttribDivisor(glGetAttribLocation(program, "y"), 1);
    glVertexAttribDivisor(glGetAttribLocation(program, "width"), 1);
    glVertexAttribDivisor(glGetAttribLocation(program, "height"), 1);
    glVertexAttribDivisor(glGetAttribLocation(program, "rotation"), 1);
    MO_CHECK_GL_ERROR;
  }

  int mosaicSize = tiles_.size() * sizeof(MyTile);
  if (!glBuffersUpToDate_ && !tiles_.empty()) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    MO_CHECK_GL_ERROR;
    glBufferData(GL_ARRAY_BUFFER, mosaicSize, &tiles_[0], GL_DYNAMIC_DRAW);
    MO_CHECK_GL_ERROR;
    glBuffersUpToDate_ = true;
  }

  glBindVertexArray(vao_);
  MO_CHECK_GL_ERROR;
}

void MosaicRendererOutline::getUniformLocations() {
  GLint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);
  viewPortWidth_ = glGetUniformLocation(program, "viewPortWidth");
  viewPortHeight_ = glGetUniformLocation(program, "viewPortHeight");
  magnification_ = glGetUniformLocation(program, "magnification");
  numTiles_ = glGetUniformLocation(program, "numTiles");
  MO_CHECK_GL_ERROR;
} 

}

