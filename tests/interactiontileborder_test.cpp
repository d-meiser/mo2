#include <gtest/gtest.h>
#include <interactiontileborder.h>
#include <potential.h>
#include <mosaic.h>
#include <potential.h>
#include <utilities.h>

using namespace Mo;


TEST(MoInteractionTileBorder, Constructor) {
    std::unique_ptr<InteractionTileBorder> interaction;
    EXPECT_NO_THROW(interaction.reset(new InteractionTileBorder(nullptr)));
}


struct IdentityPotentialFiniteRange : public Potential {
  IdentityPotentialFiniteRange(float range) : range_(range) {}
  virtual float operator()(const float* x1, const float* x2) {
    MO_UNUSED(x1);
    MO_UNUSED(x2);
    return 1.0f;
  }
  virtual float range() const {
    return range_;
  }
  float range_;
};

struct InteractionTileBorderIdentity : public ::testing::Test {
  InteractionTileBorderIdentity() :
    interaction(std::unique_ptr<Potential>(
          new IdentityPotentialFiniteRange(-1.0f))),
    mosaic(Image(150, 100), 1.4f)
  {}
  virtual void SetUp() {
    createSomeModel(2);
  }
  virtual void TearDown() {}
  void createSomeModel(int numTiles) {
    std::vector<Tile> tiles;
    for (int i = 0; i != numTiles; ++i) {
      tiles.emplace_back(
          Tile{0, 0, 0, 1.0f, std::unique_ptr<Image>(new Image(30, 20))});
    }
    mosaic.addTiles(&tiles[0], &tiles[0] + tiles.size());
  }
  InteractionTileBorder interaction;
  Mosaic mosaic;
};


TEST_F(InteractionTileBorderIdentity, InRangeBottomLeft) {
  float width = 3000.0f;
  float height = 2000.0f;
  mosaic.setTargetImage(TargetImage(Image(width, height), 1.0f));
  float range = 100.f;
  interaction.resetPotential(
      std::unique_ptr<Potential>(
        new IdentityPotentialFiniteRange(range)));
  // place tiles close to bottom left corner
  auto tile = mosaic.tilesBegin();
  tile->x_ =  -0.5f * width + 10.0f;
  tile->y_ =  -0.5f * height + 10.0f;
  ++tile;
  tile->x_ = -0.5f * width + 10.0f;
  tile->y_ = -0.5f * height + 10.0f;
  EXPECT_NE(0.0f, interaction.computeBadness(mosaic));
}

TEST_F(InteractionTileBorderIdentity, InRangeBottomRight) {
  float width = 3000.0f;
  float height = 2000.0f;
  mosaic.setTargetImage(TargetImage(Image(width, height), 1.0f));
  float range = 100.f;
  interaction.resetPotential(
      std::unique_ptr<Potential>(
        new IdentityPotentialFiniteRange(range)));
  // place tiles close to bottom left corner
  auto tile = mosaic.tilesBegin();
  tile->x_ =  0.5f * width - 50.0f;
  tile->y_ =  -0.5f * height + 30.0f;
  ++tile;
  tile->x_ = 0.5f * width - 50.0f;
  tile->y_ = -0.5f * height + 30.0f;
  EXPECT_NE(0.0f, interaction.computeBadness(mosaic));
}

TEST_F(InteractionTileBorderIdentity, InRangeTopRight) {
  float width = 3000.0f;
  float height = 2000.0f;
  mosaic.setTargetImage(TargetImage(Image(width, height), 1.0f));
  float range = 100.f;
  interaction.resetPotential(
      std::unique_ptr<Potential>(
        new IdentityPotentialFiniteRange(range)));
  // place tiles close to bottom left corner
  auto tile = mosaic.tilesBegin();
  tile->x_ = 0.5f * width - 50.0f;
  tile->y_ = 0.5f * height - 30.0f;
  ++tile;
  tile->x_ = 0.5f * width - 50.0f;
  tile->y_ = 0.5f * height - 30.0f;
  EXPECT_NE(0.0f, interaction.computeBadness(mosaic));
}


TEST_F(InteractionTileBorderIdentity, InRangeTopLeft) {
  float width = 3000.0f;
  float height = 2000.0f;
  mosaic.setTargetImage(TargetImage(Image(width, height), 1.0f));
  float range = 100.f;
  interaction.resetPotential(
      std::unique_ptr<Potential>(
        new IdentityPotentialFiniteRange(range)));
  // place tiles close to bottom left corner
  auto tile = mosaic.tilesBegin();
  tile->x_ = -0.5f * width + 50.0f;
  tile->y_ = 0.5f * height - 30.0f;
  ++tile;
  tile->x_ = -0.5f * width + 50.0f;
  tile->y_ = 0.5f * height - 30.0f;
  EXPECT_NE(0.0f, interaction.computeBadness(mosaic));
}

TEST_F(InteractionTileBorderIdentity, OutOfRange) {
  float width = 3000.0f;
  float height = 2000.0f;
  mosaic.setTargetImage(TargetImage(Image(width, height), 1.0f));
  float range = 100.f;
  interaction.resetPotential(
      std::unique_ptr<Potential>(
        new IdentityPotentialFiniteRange(range)));
  // place tiles close to bottom left corner
  auto tile = mosaic.tilesBegin();
  tile->x_ = 0.0f;
  tile->y_ = 0.0f;
  ++tile;
  tile->x_ = 0.0f;
  tile->y_ = 0.0f;
  EXPECT_EQ(0.0f, interaction.computeBadness(mosaic));
}

TEST_F(InteractionTileBorderIdentity, TwoTilesTwiceAsBadAsOne) {
  float width = 3000.0f;
  float height = 2000.0f;
  mosaic.setTargetImage(TargetImage(Image(width, height), 1.0f));
  float range = 100.f;
  interaction.resetPotential(
      std::unique_ptr<Potential>(
        new IdentityPotentialFiniteRange(range)));

  ASSERT_EQ(2, mosaic.size());

  // Place tile in bottom left corner so we have a non-zero
  // badness.
  auto tile = mosaic.tilesBegin();
  tile->x_ = -0.5f * width + 10.0f;
  tile->y_ = -0.5f * height + 10.0f;
  ++tile;
  tile->x_ = -0.5f * width + 10.0f;
  tile->y_ = -0.5f * height + 10.0f;

  float badnessTwoTiles = interaction.computeBadness(mosaic);

  mosaic.reduceSize(1);
  ASSERT_EQ(1, mosaic.size());
  float badnessOneTile = interaction.computeBadness(mosaic);

  EXPECT_FLOAT_EQ(badnessTwoTiles, 2.0f * badnessOneTile);
}

