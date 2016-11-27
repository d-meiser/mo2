include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${gtest_SOURCE_DIR}/include
    )

add_library(test_utilities
    test_utilities.cpp
    )
generate_export_header(test_utilities)

set(TESTS
    image_test
    mosaic_test
    run_configuration_test
    tile_test
   )

foreach(t ${TESTS})
  add_executable(${t} ${t}.cpp)
  target_link_libraries(${t} mo_lib test_utilities gtest_main)
  add_test(${t} ${t})
endforeach()

set(TEST_FILES
    test_files/test_image_1.jpg
    )
foreach(f ${TEST_FILES})
    file(COPY ${f} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_files)
endforeach()
