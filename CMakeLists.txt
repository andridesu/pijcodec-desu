cmake_minimum_required(VERSION 3.4...3.18)
project(pybindtest)

# add pybind11 (either as submodule or installed via find_package)
add_subdirectory(pybind11)                    # if you have the subfolder
# or: find_package(pybind11 REQUIRED)

find_package(OpenCV REQUIRED)                 # locate OpenCV
find_package(ZLIB REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})   # add include paths
include_directories(${CMAKE_SOURCE_DIR}/include)

pybind11_add_module(pypij src/main.cpp)           # build the extension
target_link_libraries(pypij PRIVATE ${OpenCV_LIBS})  # link OpenCV libs
target_sources(pypij PRIVATE
    src/PIJCodec.cpp
)
