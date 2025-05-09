cmake_minimum_required(VERSION 3.4...3.18)
project(pybindtest)

find_package(pybind11 REQUIRED)
find_package(OpenCV   REQUIRED)
find_package(ZLIB     REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

# Create shared library for common code
add_library(pijcodec SHARED
    src/PIJCodec.cpp
)
target_link_libraries(pijcodec
    PUBLIC
      ${OpenCV_LIBS}
      ${ZLIB_LIBRARIES}
)

# Python extension
pybind11_add_module(pypij
    src/main.cpp
)
target_link_libraries(pypij 
    PRIVATE 
      pijcodec
)

# standalone viewer
add_executable(pij_viewer
    src/pij_viewer.cpp
)
target_link_libraries(pij_viewer
    PRIVATE
      pijcodec
)

# standalone converter
add_executable(pij_convert 
    src/pij_convert.cpp
)
target_link_libraries(pij_convert
    PRIVATE
      pijcodec
)
