INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLFW_SOURCE_DIR}/include
    )

find_package(JPEG)

add_library(mo_lib
    badnesscomposite.cpp
    framebuffer.cpp
    image.cpp
    initial_guess.cpp
    interactionhelpers.cpp
    interactiontileborder.cpp
    interactiontiletile.cpp
    lanczos.cpp
    lennardjones.cpp
    gl3w.c
    mosaic.cpp
    mosaic_renderer_outline.cpp
    mosaic_renderer_targetimage.cpp
    mosaic_renderer_textured.cpp
    mosaic_match.cpp
    potentiallennardjones.cpp
    renderer.cpp
    run_configuration.cpp
    tile.cpp
    )
target_link_libraries(mo_lib
    ${JPEG_LIBRARIES}
    glfw
    )

add_executable(mo2 mo2.cpp)
target_link_libraries(mo2 mo_lib)
if(UNIX)
  target_link_libraries(mo2 dl)
endif()

if (MO_ENABLE_OPENGL_TESTS AND NOT MO_WITH_ADDRESS_SANITIZER)
  add_test(mo2 mo2
      -t ${PROJECT_SOURCE_DIR}/tests/test_files/test_image_1.jpg
      -n 3
      ${PROJECT_SOURCE_DIR}/tests/test_files/test_image_1.jpg
      ${PROJECT_SOURCE_DIR}/tests/test_files/test_image_1.jpg
      )
endif ()

