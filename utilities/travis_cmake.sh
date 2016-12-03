#!bin/sh

cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DMO_ENABLE_COVERAGE=ON \
  -DMO_BUILD_TESTS=ON \
  -DBUILD_GTEST=ON \
  -DBUILD_GMOCK=OFF \
  -Dgtest_disable_pthreads=ON \
  -Dgtest_hide_internal_symbols=ON \
  -DBUILD_SHARED_LIBS=ON \
  -DMO_WITH_ADDRESS_SANITIZER=ON \
  -DMO_WITH_WALL_FLAGS=ON \
  -DMO_WITH_PEDANTIC_FLAGS=ON \
  -DMO_WITH_WARNING_AS_ERRORS=ON \
  -DGLFW_BUILD_EXAMPLES=OFF \
  -DGLFW_BUILD_TESTS=OFF \
  -DGLFW_BUILD_DOCS=OFF \
  -DGLFW_INSTALL=OFF -DMO_ENABLE_OPENGL_TESTS=OFF \
  ..

