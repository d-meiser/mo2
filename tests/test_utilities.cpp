#include <test_utilities.h>
#include <mo2_config.h>

namespace Mo {
namespace Testing {

std::string testFile(const char* basename) {
  std::string f(MO_TEST_ROOT_DIR);
  f += "/test_files/";
  f += basename;
  return f;
}

}
}
