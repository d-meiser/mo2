#ifndef RUN_CONFIGURATION_H
#define RUN_CONFIGURATION_H

#include <string>
#include <vector>

#include <mo_lib_export.h>


namespace Mo {

class MO_LIB_EXPORT RunConfiguration {
public:
  RunConfiguration(int n, const char *argv[]);

  std::string* targetImageName();
  std::string* outputImageName();
  std::string* tileBegin();
  std::string* tileEnd();
  int getTargetWidth() const;
  int getTargetHeight() const;
  int getNumIter() const;
  std::string usage() const;

private:
  int targetWidth_;
  int targetHeight_;
  int numIter_;
  std::string targetImageName_;
  std::string outputImageName_;
  std::vector<std::string> tiles_;
};

}

#endif

