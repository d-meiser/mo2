#include <run_configuration.h>
#include <stdexcept>


namespace Mo {

RunConfiguration::RunConfiguration(int n, const char *argv[]) {
  if (n < 1) {
    throw std::runtime_error("Invalid command line arguments.");
  }
  std::vector<int> used(n, 0);

  int i;
  for (i = 1; i != n; ++i) {
    if (argv[i] && std::string(argv[i]) == "-t") {
      if (i == n - 1) {
        throw std::runtime_error("Invalid target image.");
      }
      targetImageName_ = argv[i + 1];
      used[i] = 1;
      used[i + 1] = 1;
      break;
    }
  }
  if (i == n) {
    throw std::runtime_error("Invalid target image.");
  }

  // Done parsing options, now obtain the tile file names.
  for (i = 1; i != n; ++i) {
    if (!used[i]) {
      tiles_.push_back(argv[i]);
      used[i] = 1;
    }
  }
  if (tiles_.empty()) {
    throw std::runtime_error("Must provide at least one tile.");
  }

  // Generated output file name if none provided.
  if (outputImageName_.empty()) {
    outputImageName_ = std::string("out_") + targetImageName_;
  }
}

std::string* RunConfiguration::tileBegin() {
  // The only way to construct a RunConfiguration is
  // with more than one tile.  Hence this is legal.
  return &tiles_[0];
}

std::string* RunConfiguration::tileEnd() {
  // The only way to construct a RunConfiguration is
  // with more than one tile.  Hence this is legal.
  return &tiles_[tiles_.size() - 1] + 1;
}

std::string* RunConfiguration::targetImageName() {
  return &targetImageName_;
}

std::string* RunConfiguration::outputImageName() {
  return &outputImageName_;
}

}

