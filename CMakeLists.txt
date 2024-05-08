cmake_minimum_required(VERSION 3.20)
project(raytracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

file(GLOB SOURCES "sources/*.cpp" "sources/**/*.cpp")

add_executable(raytracer ${SOURCES})
target_include_directories(raytracer PUBLIC "include")

option(BUILD_DOC "Build documentation" ON)

find_package(Doxygen)
if (DOXYGEN_FOUND AND BUILD_DOC)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message(STATUS "Build documentation: ${BUILD_DOC}")

    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        COMMAND make -C ${CMAKE_CURRENT_BINARY_DIR}/doxygen/latex
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/doxygen/latex/refman.pdf ${CMAKE_CURRENT_SOURCE_DIR}/docs/raytracer.pdf
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else (DOXYGEN_FOUND AND BUILD_DOC)
    message("Please install Doxygen to generate the documentation")
endif (DOXYGEN_FOUND AND BUILD_DOC)

add_subdirectory("library/config")
target_link_libraries(raytracer config++)
