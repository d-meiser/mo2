#ifndef MOSAIC_RENDERER_TEXTURED_H
#define MOSAIC_RENDERER_TEXTURED_H

#include <mo_lib_export.h>
#include <mosaic_renderer.h>
#include <tile.h>
#include <GL/gl3w.h>
#include <vector>


namespace Mo {

class MO_LIB_EXPORT MosaicRendererTextured : public MosaicRenderer {
public:
  MosaicRendererTextured(int renderTargetWidth, int renderTargetHeight);
  ~MosaicRendererTextured();
  void setMosaic(const Mosaic& mosaic) override;
  void setTileImages(const std::vector<Tile>& tiles) override;

private:
  GLuint vbo_;
  GLuint vao_;
  GLuint tileTextures_;
  bool glBuffersUpToDate_;
  int width_;
  int height_;

  struct MyTile {
    float x_;
    float y_;
    float angle_;
    float width_;
    float height_;
  };
  std::vector<MyTile> tiles_;

  // UniformLocations
  float viewPortWidth_;
  float viewPortHeight_;
  float magnification_;
  float numTiles_;

  const char* vertexShaderSource() override;
  const char* fragmentShaderSource() override;
  void bindVAO() override;
  void draw() override;

  void setupVAO();
  void getUniformLocations();
  void createTileTextures();
};

}

#endif

