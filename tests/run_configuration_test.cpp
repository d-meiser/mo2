#include <gtest/gtest.h>
#include <run_configuration.h>
#include <stdexcept>


TEST(RunConfiguration, IncludeTest) {}

TEST(RunConfiguration, MustProvideAtLeastOneArgument) {
  const char *argv[] = {0};
  EXPECT_THROW(Mo::RunConfiguration(0, argv), std::runtime_error);
}

TEST(RunConfiguration, MustProvideTargetImage) {
  const char *argv[] = {"blah"};
  EXPECT_THROW(Mo::RunConfiguration(1, argv), std::runtime_error);
}

TEST(RunConfiguration, MustProvideAtLeastOneTile) {
  const char *argv[] = {"blah", "-t", "targetImage.jpg"};
  EXPECT_THROW(Mo::RunConfiguration(3, argv), std::runtime_error);
}

TEST(RunConfiguration, HeresALegalConfiguration) {
  const char *argv[] = {"blah",
      "-t", "targetImage.jpg",
      "aTile.jpg",
      "anotherTile.jpg"};
  Mo::RunConfiguration config(5, argv);
  EXPECT_EQ(2, std::distance(config.tileBegin(), config.tileEnd()));
  ASSERT_NE(nullptr, config.targetImageName());
  EXPECT_EQ(std::string(argv[2]), *config.targetImageName());
}

TEST(RunConfiguration, OutputNameIsGeneratedIfNotSupplied) {
  const char *argv[] = {"blah",
      "-t", "targetImage.jpg",
      "aTile.jpg",
      "anotherTile.jpg"};
  Mo::RunConfiguration config(5, argv);
  ASSERT_NE(nullptr, config.outputImageName());
  EXPECT_FALSE(config.outputImageName()->empty());
}

