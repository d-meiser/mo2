#include <mosaic_renderer_outline.h>
#include <tile.h>
#include <utilities.h>
#include <mosaic.h>


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
    "uniform float viewPortWidth = 1.0f;\n"
    "uniform float viewPortHeight = 1.0f;\n"
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
    "void main(void)\n"
    "{\n"
    "    vec2 offset = pos[gl_VertexID];\n"
    "    float c = cos(rotation);\n"
    "    float s = sin(rotation);\n"
    "    float x_ = (x + width * offset.x);\n"
    "    float y_ = (y + height * offset.y);\n"
    "    gl_Position = vec4(2.0f * magnification * (c * x_ - s * y_) / viewPortWidth,\n"
    "                       2.0f * magnification * (s * x_ + c * y_) / viewPortHeight,\n"
    "                       (gl_InstanceID - numTiles) / numTiles,\n"
    "                       1.0f);\n"
    "}\n"
    ;

static const char fShaderSource[] =
    "#version 330\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    gl_FragColor = vec4(0.3f, 0.3f, 0.7f, 0.3f);\n"
    "}\n"
    ;

MosaicRendererOutline::MosaicRendererOutline() :
    bufferSize_(0),
    mosaic_(nullptr),
    vbo_(0),
    vao_(0) {
}

MosaicRendererOutline::~MosaicRendererOutline() {
  glDeleteBuffers(1, &vbo_);
  glDeleteVertexArrays(1, &vao_);
}

void MosaicRendererOutline::setMosaic(Mosaic* mosaic) {
  mosaic_ = mosaic;
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

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mosaic_->size());
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

    glVertexAttribPointer(0, 1, GL_FLOAT, false, sizeof(Tile),
        reinterpret_cast<void*>(0 * sizeof(float)));
    glVertexAttribPointer(1, 1, GL_FLOAT, false, sizeof(Tile),
        reinterpret_cast<void*>(1 * sizeof(float)));
    glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(Tile),
        reinterpret_cast<void*>(2 * sizeof(float)));
    glVertexAttribPointer(3, 1, GL_FLOAT, false, sizeof(Tile),
        reinterpret_cast<void*>(3 * sizeof(float)));
    glVertexAttribPointer(4, 1, GL_FLOAT, false, sizeof(Tile),
        reinterpret_cast<void*>(3 * sizeof(float)));
    MO_CHECK_GL_ERROR;
  }

  int mosaicSize = mosaic_->size() * sizeof(Mo::Tile);
  if (bufferSize_ != mosaicSize) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    MO_CHECK_GL_ERROR;
    auto t = mosaic_->cTilesBegin();
    glBufferData(GL_ARRAY_BUFFER, mosaicSize, &*t, GL_STATIC_DRAW);
    MO_CHECK_GL_ERROR;
    bufferSize_ = mosaicSize;
  }

#if 0
  glGenTextures(1, &tileTextures_);
  glBindTexture(GL_TEXTURE_2D_ARRAY, tileTextures_);
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 512, 512, 100);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  //glTexSubImage3D(GL_TEXTURE_2D_ARRAY, ...);
  MO_CHECK_GL_ERROR;
#endif
}

}

