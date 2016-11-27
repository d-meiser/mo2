INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

find_package(JPEG)

add_library(mo_lib
    image.cpp
    )
target_link_libraries(mo_lib
    ${JPEG_LIBRARIES}
    )

