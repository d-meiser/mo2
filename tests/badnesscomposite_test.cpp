#include <gtest/gtest.h>
#include <badnesscomposite.h>
#include <mosaic.h>

#include <memory>


using namespace Mo;


TEST(BadnessComposite, Constructor) {
    std::unique_ptr<BadnessComposite> badness;
    EXPECT_NO_THROW(badness.reset(new BadnessComposite));
}

#if 0

struct MoBadnessCompositeT : public ::testing::Test {
    MoBadnessCompositeT()  :
        model(),
        targetImage(QImage(), QSize(400, 300))
    {}
    virtual ~MoBadnessCompositeT() {}
    virtual void SetUp() {
        createSomeModel(2);
    }
    virtual void TearDown() { }
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
    MoMosaicModel model;
    MoTargetImage targetImage;
};

TEST_F(MoBadnessCompositeT, EmptyCompositeHasZeroBadness) {
    MoBadnessComposite badness;
    EXPECT_EQ(0.0f, badness.computeBadness(model, targetImage));
}

class ConstantBadness : public MoBadness {
public:
    ConstantBadness(float badness) : badness_(badness) {}
    virtual ~ConstantBadness() {}
    float computeBadness(const MoMosaicModel &model, const MoTargetImage &targetImage) {
        Q_UNUSED(model);
        Q_UNUSED(targetImage);
        return badness_;
    }
private:
    float badness_;
};

TEST_F(MoBadnessCompositeT, NonEmptyBadnessHasNonZeroBadness) {
    MoBadnessComposite badness;
    badness.addBadness(std::unique_ptr<MoBadness>(new ConstantBadness(2.3f)));
    EXPECT_NE(0.0f, badness.computeBadness(model, targetImage));
}

TEST_F(MoBadnessCompositeT, BadnessesAddUp) {
    MoBadnessComposite badness;
    float b1 = 2.3f;
    badness.addBadness(std::unique_ptr<MoBadness>(new ConstantBadness(b1)));
    float b2 = 5.2f;
    badness.addBadness(std::unique_ptr<MoBadness>(new ConstantBadness(b2)));
    EXPECT_FLOAT_EQ(b1 + b2, badness.computeBadness(model, targetImage));
}

#endif
