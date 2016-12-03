#include <gtest/gtest.h>
#include <interactiontiletile.h>
#include <mosaic.h>
#include <targetimage.h>
#include <tile.h>
#include <potential.h>
#include <utilities.h>

#include <memory>
#include <cmath>


using namespace Mo;


TEST(InteractionTileTile, Constructor) {
    std::unique_ptr<InteractionTileTile> interaction;
    EXPECT_NO_THROW(interaction.reset(new InteractionTileTile(nullptr)));
}


class IdentityPotential : public Potential {
    virtual float operator()(const float* x1, const float* x2) {
        MO_UNUSED(x1);
        MO_UNUSED(x2);
        return 1.0f;
    }
    virtual float range() const {
        return -1.0f;
    }
};


struct InteractionTileTileIdentity : public ::testing::Test {
    InteractionTileTileIdentity() :
        interaction(std::unique_ptr<Potential>(new IdentityPotential())),
        mosaic(Image(150, 100), 1.4f),
        targetImage(mosaic.targetImage())
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
    InteractionTileTile interaction;
    Mosaic mosaic;
    TargetImage targetImage;
};


TEST_F(InteractionTileTileIdentity, CanComputeBadness) {
    EXPECT_NO_THROW(interaction.computeBadness(mosaic, targetImage));
}

TEST_F(InteractionTileTileIdentity, UnitScaleTilesGiveBadnessOfOne) {
    EXPECT_NEAR(1.0f, interaction.computeBadness(mosaic, targetImage), 1.0e-5f);
}

TEST_F(InteractionTileTileIdentity, ThreeTilesGiveThreeTimesAsMuchBadness) {
    int numTiles = 3;
    mosaic.clear();
    createSomeModel(numTiles);
    EXPECT_NEAR(3.0f, interaction.computeBadness(mosaic, targetImage), 1.0e-5f);
}

TEST_F(InteractionTileTileIdentity, FourTilesGiveSixTimesAsMuchBadness) {
    int numTiles = 4;
    mosaic.clear();
    createSomeModel(numTiles);
    EXPECT_NEAR(6.0f, interaction.computeBadness(mosaic, targetImage), 1.0e-5f);
}

class IdentityPotentialFiniteRange : public IdentityPotential {
public:
    IdentityPotentialFiniteRange(float range) : range_(range) {}
    virtual float range() const {
        return range_;
    }
    float range_;
};

TEST_F(InteractionTileTileIdentity, FiniteRangeInRange) {
    float range = 100.0f;
    interaction.resetPotential(
                std::unique_ptr<Potential>(
                    new IdentityPotentialFiniteRange(range)));
    auto tile = mosaic.tilesBegin();
    tile->x_ = 0.0f;
    tile->y_ = 0.2f;
    ++tile;
    tile->x_ = 0.0f;
    tile->y_ = 0.2f;
    EXPECT_NEAR(1.0f, interaction.computeBadness(mosaic, targetImage), 1.0e-5f);
}

TEST_F(InteractionTileTileIdentity, FiniteRangeMarginal) {
    float range = 100.0f;
    interaction.resetPotential(
                std::unique_ptr<Potential>(
                    new IdentityPotentialFiniteRange(range)));

    auto tile = mosaic.tilesBegin();
    float radius1 = std::sqrt(tile->width() * tile->width() +
        tile->height() * tile->height());
    ++tile;
    float radius2 = std::sqrt(tile->width() * tile->width() +
        tile->height() * tile->height());

    float distance = range + radius1 + radius2 - 1.0f;

    float alpha = 1.0f;
    tile = mosaic.tilesBegin();
    tile->x_ = 0.0f;
    tile->y_ = 0.0f;
    ++tile;
    tile->x_ = cos(alpha) * distance;
    tile->y_ = -sin(alpha) * distance;

    EXPECT_NEAR(1.0f, interaction.computeBadness(mosaic, targetImage), 1.0e-5f);
}

TEST_F(InteractionTileTileIdentity, FiniteRangeOutOfRange) {
    float range = 100.0f;
    interaction.resetPotential(
                std::unique_ptr<Potential>(
                    new IdentityPotentialFiniteRange(range)));

    auto tile = mosaic.tilesBegin();
    float radius1 = std::sqrt(tile->width() * tile->width() +
        tile->height() * tile->height());
    ++tile;
    float radius2 = std::sqrt(tile->width() * tile->width() +
        tile->height() * tile->height());

    float distance = range + radius1 + radius2 + 1.0f;

    float alpha = 1.0f;
    tile = mosaic.tilesBegin();
    tile->x_ = 0.0f;
    tile->y_ = 0.0f;
    ++tile;
    tile->x_ = cos(alpha) * distance;
    tile->y_ = -sin(alpha) * distance;

    EXPECT_NEAR(0.0f, interaction.computeBadness(mosaic, targetImage), 1.0e-5f);
}

