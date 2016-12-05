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
    interactionhelpers.cpp
    interactiontileborder.cpp
    interactiontiletile.cpp
    gl3w.c
    mosaic.cpp
    renderer.cpp
    run_configuration.cpp
    tile.cpp
    )
target_link_libraries(mo_lib
    ${JPEG_LIBRARIES}
    )

add_executable(mo2 mo2.cpp)
target_link_libraries(mo2 mo_lib)
if(UNIX)
  target_link_libraries(mo2 dl)
endif()

add_test(mo2 mo2
    -t ${PROJECT_SOURCE_DIR}/tests/test_files/test_image_1.jpg
    ${PROJECT_SOURCE_DIR}/tests/test_files/test_image_1.jpg
    ${PROJECT_SOURCE_DIR}/tests/test_files/test_image_1.jpg
    )

