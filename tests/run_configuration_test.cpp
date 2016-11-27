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

