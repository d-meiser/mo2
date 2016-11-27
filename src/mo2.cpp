#include <vector>
#include <string>

#include <mo2.h>
using namespace Mo;



int main(int argn, const char *argv[]) {
  RunConfiguration config(argn, argv);

  Mosaic mosaic{
      *config.targetImageName(),
      config.tileBegin(),
      config.tileEnd()};

  return 0;
}


