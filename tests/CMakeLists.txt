include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}/include
    )

set(TESTS
    image_test
   )

foreach(t ${TESTS})
  add_executable(${t} ${t}.cpp)
  target_link_libraries(${t} gtest_main)
  add_test(${t} ${t})
endforeach()

