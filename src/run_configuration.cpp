#include <run_configuration.h>
#include <stdexcept>
#include <sstream>


namespace Mo {

std::string RunConfiguration::usage() const {
  return
  "-t targetImageName :  File name of image used for mosaic target.\n"
  "-o outputImageName :  Output file name.\n"
  "-w mosaic width    :  Width of mosaic produced.\n"
  "-h mosaic height   :  Height of mosaic produced.\n";
}

template <typename T>
void getValue(const char* option, T* value);

template <>
void getValue(const char* option, std::string* value) {
  *value = option;
}

template <>
void getValue(const char* option, int* value) {
  std::stringstream ss(option);
  ss >> *value;
}

template<typename T>
void parseOption(bool required, int argn, const char *argv[], int *used,
                 const std::string& option, T* value) {
  int i;
  for (i = 1; i != argn; ++i) {
    if (!used[i] && !used[i + 1] && std::string(argv[i]) == option) {
      if (i == argn - 1) {
        throw std::runtime_error("Invalid option.");
      }
      getValue(argv[i + 1], value);
      ++used[i];
      ++used[i + 1];
      break;
    }
  }
  if (required && i == argn) {
    throw std::runtime_error("Invalid option.");
  }
}

RunConfiguration::RunConfiguration(int n, const char *argv[]) {
  targetWidth_ = -1;
  targetHeight_ = -1;

  if (n < 1) {
    throw std::runtime_error("Invalid command line arguments.");
  }
  std::vector<int> used(n, 0);

  parseOption(true, n, argv, &used[0], "-t", &targetImageName_);
  parseOption(false, n, argv, &used[0], "-o", &outputImageName_);
  parseOption(false, n, argv, &used[0], "-w", &targetWidth_);
  parseOption(false, n, argv, &used[0], "-h", &targetHeight_);

  // Done parsing options, now obtain the tile file names.
  for (int i = 1; i != n; ++i) {
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

