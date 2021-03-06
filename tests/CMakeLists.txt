include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/include
    ${PROJECT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${gtest_SOURCE_DIR}/include
    ${GLFW_SOURCE_DIR}/include
    )

add_library(test_utilities
    test_utilities.cpp
    )
generate_export_header(test_utilities)

set(TESTS
    badnesscomposite_test
    image_test
    image_scaling_test
    interactiontileborder_test
    interactiontiletile_test
    mosaic_test
    quadrature_test
    run_configuration_test
    tile_test
   )

foreach(t ${TESTS})
  add_executable(${t} ${t}.cpp)
  target_link_libraries(${t} mo_lib test_utilities gtest_main)
  if(UNIX)
    target_link_libraries(${t} dl)
  endif()
  add_test(${t} ${t})
endforeach()

set(OPENGL_TESTS
    glfw_test
    gl3w_test
    framebuffer_test
    mosaic_match_test
    mosaicrenderer_test
    mosaicrendereroutline_test
    mosaicrenderertextured_test
    renderer_test
    )
foreach(t ${OPENGL_TESTS})
  add_executable(${t} ${t}.cpp)
  target_link_libraries(${t} mo_lib test_utilities glfw gtest_main)
  if(UNIX)
    target_link_libraries(${t} dl)
  endif()
endforeach()
if (MO_ENABLE_OPENGL_TESTS AND NOT MO_WITH_ADDRESS_SANITIZER)
  foreach(t ${OPENGL_TESTS})
    add_test(${t} ${t})
  endforeach()
endif ()

set(TEST_FILES
    test_files/test_image_1.jpg
    test_files/passThroughRenderer_master.jpg
    test_files/test_image_1_upsampled_master.jpg
    test_files/test_image_1_downsampled_master.jpg
    test_files/mosaicOutlineRenderer_master.jpg
    test_files/mosaicTexturedRenderer_master.jpg
    )
foreach(f ${TEST_FILES})
    file(COPY ${f} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_files)
endforeach()
