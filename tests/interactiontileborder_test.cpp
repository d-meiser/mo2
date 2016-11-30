#include <gtest/gtest.h>
#include <interactiontileborder.h>
#include <potential.h>
#include <mosaic.h>
#include <potential.h>

using namespace Mo;


TEST(MoInteractionTileBorder, Constructor) {
    std::unique_ptr<InteractionTileBorder> interaction;
    EXPECT_NO_THROW(interaction.reset(new InteractionTileBorder(nullptr)));
}

#if 0

class IdentityPotentialFiniteRange : public MoPotential {
public:
    IdentityPotentialFiniteRange(float range) : range_(range) {}
    virtual float operator()(const float* x1, const float* x2) {
        Q_UNUSED(x1);
        Q_UNUSED(x2);
        return 1.0f;
    }
    virtual float range() const {
        return range_;
    }
    float range_;
};

struct MoInteractionTileBorderIdentity : public ::testing::Test {
    MoInteractionTileBorderIdentity() :
        interaction(std::unique_ptr<MoPotential>(
                        new IdentityPotentialFiniteRange(-1.0f))),
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
        std::vector<float> scales(numTiles, 1.0f);
        model.setScales(&scales[0], &scales[0] + numTiles);
    }
    MoInteractionTileBorder interaction;
    MoMosaicModel model;
    MoTargetImage targetImage;
};

TEST_F(MoInteractionTileBorderIdentity, InRangeBottomLeft) {
    float width = 3000.0f;
    float height = 2000.0f;
    targetImage = MoTargetImage(QImage(), QSize(width, height));
    float range = 100.f;
    interaction.resetPotential(
                std::unique_ptr<MoPotential>(
                    new IdentityPotentialFiniteRange(range)));
    // place tiles close to bottom left corner
    model.getXCoords()[0] = -0.5f * width + 10.0f;
    model.getXCoords()[1] = -0.5f * width + 10.0f;
    model.getYCoords()[0] = -0.5f * height + 10.0f;
    model.getYCoords()[1] = -0.5f * height + 10.0f;
    EXPECT_NE(0.0f, interaction.computeBadness(model, targetImage));
}

TEST_F(MoInteractionTileBorderIdentity, InRangeBottomRight) {
    float width = 3000.0f;
    float height = 2000.0f;
    targetImage = MoTargetImage(QImage(), QSize(width, height));
    float range = 100.f;
    interaction.resetPotential(
                std::unique_ptr<MoPotential>(
                    new IdentityPotentialFiniteRange(range)));
    // place tiles close to the top right corner
    model.getXCoords()[0] = 0.5f * width - 50.0f;
    model.getXCoords()[1] = 0.5f * width - 50.0f;
    model.getYCoords()[0] = -0.5f * height + 30.0f;
    model.getYCoords()[1] = -0.5f * height + 30.0f;
    EXPECT_NE(0.0f, interaction.computeBadness(model, targetImage));
}

TEST_F(MoInteractionTileBorderIdentity, InRangeTopRight) {
    float width = 3000.0f;
    float height = 2000.0f;
    targetImage = MoTargetImage(QImage(), QSize(width, height));
    float range = 100.f;
    interaction.resetPotential(
                std::unique_ptr<MoPotential>(
                    new IdentityPotentialFiniteRange(range)));
    // place tiles close to the top right corner
    model.getXCoords()[0] = 0.5f * width - 50.0f;
    model.getXCoords()[1] = 0.5f * width - 50.0f;
    model.getYCoords()[0] = 0.5f * height - 30.0f;
    model.getYCoords()[1] = 0.5f * height - 30.0f;
    EXPECT_NE(0.0f, interaction.computeBadness(model, targetImage));
}

TEST_F(MoInteractionTileBorderIdentity, InRangeTopLeft) {
    float width = 3000.0f;
    float height = 2000.0f;
    targetImage = MoTargetImage(QImage(), QSize(width, height));
    float range = 100.f;
    interaction.resetPotential(
                std::unique_ptr<MoPotential>(
                    new IdentityPotentialFiniteRange(range)));
    // place tiles close to the top right corner
    model.getXCoords()[0] = -0.5f * width + 10.0f;
    model.getXCoords()[1] = -0.5f * width + 10.0f;
    model.getYCoords()[0] = 0.5f * height - 30.0f;
    model.getYCoords()[1] = 0.5f * height - 30.0f;
    EXPECT_NE(0.0f, interaction.computeBadness(model, targetImage));
}

TEST_F(MoInteractionTileBorderIdentity, OutOfRange) {
    float width = 3000.0f;
    float height = 2000.0f;
    targetImage = MoTargetImage(QImage(), QSize(width, height));
    float range = 100.0f;
    interaction.resetPotential(
                std::unique_ptr<MoPotential>(
                    new IdentityPotentialFiniteRange(range)));
    // place tiles at the origin
    model.getXCoords()[0] = 0.0f;
    model.getXCoords()[1] = 0.0f;
    model.getYCoords()[0] = 0.0f;
    model.getYCoords()[1] = 0.0f;
    EXPECT_EQ(0.0f, interaction.computeBadness(model, targetImage));
}

TEST_F(MoInteractionTileBorderIdentity, TwoTilesTwiceAsBadAsOne) {
    float width = 3000.0f;
    float height = 2000.0f;
    targetImage = MoTargetImage(QImage(), QSize(width, height));
    float range = 100.f;
    interaction.resetPotential(
                std::unique_ptr<MoPotential>(
                    new IdentityPotentialFiniteRange(range)));

    ASSERT_EQ(2, model.size());

    // Place tile in bottom left corner so we have a non-zero
    // badness.
    model.getXCoords()[0] = -0.5f * width + 10.0f;
    model.getXCoords()[1] = -0.5f * width + 10.0f;
    model.getYCoords()[0] = -0.5f * height + 10.0f;
    model.getYCoords()[1] = -0.5f * height + 10.0f;

    float badnessTwoTiles = interaction.computeBadness(model, targetImage);

    model.resize(1);
    ASSERT_EQ(1, model.size());
    float badnessOneTile = interaction.computeBadness(model, targetImage);

    EXPECT_FLOAT_EQ(badnessTwoTiles, 2.0f * badnessOneTile);
}

#endif
