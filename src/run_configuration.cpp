#include <run_configuration.h>
#include <stdexcept>


namespace Mo {

RunConfiguration::RunConfiguration(int n, const char *argv[]) {
  if (n < 1) {
    throw std::runtime_error("Invalid command line arguments.");
  }

  int i;
  for (i = 1; i != n; ++i) {
    if (argv[i] && std::string(argv[i]) == "-t") {
      if (i == n - 1) {
        throw std::runtime_error("Invalid target image.");
      }
      targetImageName_ = argv[i + 1];
      argv[i] = 0;
      argv[i + 1] = 0;
      break;
    }
  }
  if (i == n) {
    throw std::runtime_error("Invalid target image.");
  }

  // Done parsing options, now obtain the tile file names.
  for (i = 1; i != n; ++i) {
    if (argv[i]) {
      tiles_.push_back(argv[i]);
    }
  }
  if (tiles_.empty()) {
    throw std::runtime_error("Must provide at least one tile.");
  }
}

}

