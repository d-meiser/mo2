#include <gtest/gtest.h>
#include <interactiontiletile.h>
#include <mosaic.h>
#include <targetimage.h>
#include <tile.h>
#include <potential.h>

#include <memory>
#include <cmath>


using namespace Mo;


TEST(InteractionTileTile, Constructor) {
    std::unique_ptr<InteractionTileTile> interaction;
    EXPECT_NO_THROW(interaction.reset(new InteractionTileTile(nullptr)));
}

#if 0

class IdentityPotential : public MoPotential {
    virtual float operator()(const float* x1, const float* x2) {
        Q_UNUSED(x1);
        Q_UNUSED(x2);
        return 1.0f;
    }
    virtual float range() const {
        return -1.0f;
    }
};


struct InteractionTileTileIdentity : public ::testing::Test {
    InteractionTileTileIdentity() :
        interaction(std::unique_ptr<MoPotential>(new IdentityPotential())),
        model(),
        targetImage(QImage(), QSize(150, 100))
    {}
    virtual void SetUp() {
        createSomeModel(2);
    }
    virtual void TearDown() {}
    void createSomeModel(int numTiles) {
        std::vector<MoTile> tiles(numTiles,
            MoTile(QImage(20, 30, QImage::Format_RGBA8888)));
        model.constructInitialState(targetImage, tiles);
        std::vector<float> x(numTiles);
        model.setXCoords(&x[0], &x[0] + numTiles);
        std::vector<float> y(numTiles);
        model.setXCoords(&y[0], &y[0] + numTiles);
        std::vector<float> rotations(numTiles, 0.0f);
        model.setRotations(&rotations[0], &rotations[0] + numTiles);
        std::vector<float> scales(numTiles, 0.0f);
        model.setRotations(&scales[0], &scales[0] + numTiles);
    }
    InteractionTileTile interaction;
    MoMosaicModel model;
    MoTargetImage targetImage;
};


TEST_F(InteractionTileTileIdentity, CanComputeBadness) {
    EXPECT_NO_THROW(interaction.computeBadness(model, targetImage));
}

TEST_F(InteractionTileTileIdentity, UnitScaleTilesGiveBadnessOfOne) {
    EXPECT_NEAR(1.0f, interaction.computeBadness(model, targetImage), 1.0e-5f);
}

TEST_F(InteractionTileTileIdentity, ThreeTilesGiveThreeTimesAsMuchBadness) {
    int numTiles = 3;
    createSomeModel(numTiles);
    EXPECT_NEAR(3.0f, interaction.computeBadness(model, targetImage), 1.0e-5f);
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
                std::unique_ptr<MoPotential>(
                    new IdentityPotentialFiniteRange(range)));
    model.getXCoords()[0] = 0.0f;
    model.getXCoords()[1] = 0.2f;
    model.getYCoords()[0] = 0.0f;
    model.getYCoords()[1] = 0.2f;
    EXPECT_NEAR(1.0f, interaction.computeBadness(model, targetImage), 1.0e-5f);
}

TEST_F(InteractionTileTileIdentity, FiniteRangeMarginal) {
    float range = 100.0f;
    interaction.resetPotential(
                std::unique_ptr<MoPotential>(
                    new IdentityPotentialFiniteRange(range)));
    std::vector<float> w(model.size());
    model.getWidths(&w[0]);
    std::vector<float> h(model.size());
    model.getHeights(&h[0]);
    float radius1 = std::sqrt(w[0] * w[0] + h[0] * h[0]);
    float radius2 = std::sqrt(w[1] * w[1] + h[1] * h[1]);

    float distance = range + radius1 + radius2 - 1.0f;

    float alpha = 1.0f;
    model.getXCoords()[0] = 0.0f;
    model.getXCoords()[1] = cos(alpha) * distance;
    model.getYCoords()[0] = 0.0f;
    model.getYCoords()[1] = -sin(alpha) * distance;

    EXPECT_NEAR(1.0f, interaction.computeBadness(model, targetImage), 1.0e-5f);
}

TEST_F(InteractionTileTileIdentity, FiniteRangeOutOfRange) {
    float range = 100.0f;
    interaction.resetPotential(
                std::unique_ptr<MoPotential>(
                    new IdentityPotentialFiniteRange(range)));
    std::vector<float> w(model.size());
    model.getWidths(&w[0]);
    std::vector<float> h(model.size());
    model.getHeights(&h[0]);
    float radius1 = std::sqrt(w[0] * w[0] + h[0] * h[0]);
    float radius2 = std::sqrt(w[1] * w[1] + h[1] * h[1]);

    float distance = range + radius1 + radius2 + 1.0f;

    float alpha = 1.0f;
    model.getXCoords()[0] = 0.0f;
    model.getXCoords()[1] = cos(alpha) * distance;
    model.getYCoords()[0] = 0.0f;
    model.getYCoords()[1] = -sin(alpha) * distance;

    EXPECT_FLOAT_EQ(0.0f, interaction.computeBadness(model, targetImage));
}

#endif
