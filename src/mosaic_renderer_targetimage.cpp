#include <mosaic_renderer_targetimage.h>
#include <utilities.h>
#include <GLFW/glfw3.h>
#include <mosaic.h>

namespace Mo {

namespace {

GLuint createTexture(GLuint texture) {
  if (!texture || !glIsTexture(texture)) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    MO_CHECK_GL_ERROR;
  }
  return texture;
}

int nextPowerOfTwo(int m) {
  int maxPowerOfTwo = 31;
  for (int i = 1; i < maxPowerOfTwo; ++i) {
    int powerOfTwo = 1 << i;
    if (powerOfTwo >= m) return powerOfTwo;
  }
  return 1 << maxPowerOfTwo;
}

}

static const char vShaderSource[] =
    "#version 150\n"
    "\n"
    "uniform float viewPortWidth = 1000.0f;\n"
    "uniform float viewPortHeight = 1000.0f;\n"
    "\n"
    "const vec2 pos[] = vec2[4](\n"
    "  vec2(-1.0f,  1.0f),\n"
    "  vec2(-1.0f, -1.0f),\n"
    "  vec2( 1.0f,  1.0f),\n"
    "  vec2( 1.0f, -1.0f)\n"
    ");\n"
    "\n"
    "out vec2 texCoord;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    gl_Position = vec4(pos[gl_VertexID],\n"
    "                       0.0,\n"
    "                       1.0);\n"
    "    texCoord = 0.5 * (1.0 + pos[gl_VertexID]);\n"
    "}\n"
    ;

static const char fShaderSource[] =
    "#version 150\n"
    "\n"
    "uniform sampler2D texture0;\n"
    "in vec2 texCoord;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    gl_FragColor = texture(texture0,\n"
    "                           vec2(texCoord.x, 1 - texCoord.y));\n"
    "}\n"
    ;

MosaicRendererTargetImage::MosaicRendererTargetImage(
    int renderTargetWidth, int renderTargetHeight) :
    MosaicRenderer(renderTargetWidth, renderTargetHeight),
    targetImageTexture_(0),
    vao_(0)
{}

MosaicRendererTargetImage::~MosaicRendererTargetImage() {
  if (targetImageTexture_) {
    glDeleteTextures(1, &targetImageTexture_);
    targetImageTexture_ = 0;
  }
  if (vao_) {
    glDeleteVertexArrays(1, &vao_);
    vao_ = 0;
  }
}

void MosaicRendererTargetImage::setMosaic(const Mosaic& mosaic) {
  // TODO: Cache width and height of texture so we don't have
  // to reallocate texture sizes all the time.
  targetImageTexture_ = createTexture(targetImageTexture_);
  glBindTexture(GL_TEXTURE_2D, targetImageTexture_);
  MO_CHECK_GL_ERROR;

  int width = mosaic.targetImage().width();
  width = nextPowerOfTwo(width);
  int height = mosaic.targetImage().height();
  height = nextPowerOfTwo(height);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
  MO_CHECK_GL_ERROR;

  Image img(width, height);
  mosaic.targetImage().image().stretch(width, height, img.getPixelData());
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB,
                  GL_UNSIGNED_BYTE, img.getConstPixelData());
  MO_CHECK_GL_ERROR;
}

void MosaicRendererTargetImage::setTileImages(const std::vector<Tile>& tiles) {
  MO_UNUSED(tiles);
}

const char* MosaicRendererTargetImage::vertexShaderSource() {
  return vShaderSource;
}

const char* MosaicRendererTargetImage::fragmentShaderSource() {
  return fShaderSource;
}

void MosaicRendererTargetImage::bindVAO() {
  MO_CHECK_GL_ERROR;
  if (!vao_) {
    glGenVertexArrays(1, &vao_);
  }
  glBindVertexArray(vao_);
  glBindTexture(GL_TEXTURE_2D, targetImageTexture_);
  MO_CHECK_GL_ERROR;
}

void MosaicRendererTargetImage::draw() {
  MO_CHECK_GL_ERROR;
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  MO_CHECK_GL_ERROR;
}

}