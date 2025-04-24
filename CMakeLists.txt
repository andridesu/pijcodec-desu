cmake_minimum_required(VERSION 3.22)
project(pijcodec LANGUAGES CXX)

# ---------------------------------------------------------------------
# C++ flags
# ---------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------
# Python (pin to vcpkg build; enable modern FindPython inside pybind11)
# ---------------------------------------------------------------------
set(PYBIND11_FINDPYTHON ON)
set(Python3_ROOT_DIR "C:/tools/vcpkg/installed/x64-windows")

find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development)

# ---------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------
find_package(pybind11 CONFIG REQUIRED)

# Ask only for the OpenCV parts you really use
find_package(OpenCV REQUIRED COMPONENTS core imgcodecs imgproc)

find_package(ZLIB REQUIRED)

# ---------------------------------------------------------------------
# Pij codec static library (pure C++)
# ---------------------------------------------------------------------
add_library(pijcodec_static STATIC src/PIJCodec.cpp)
target_include_directories(pijcodec_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>                      # ← for later install()
)
target_link_libraries(pijcodec_static
    PUBLIC
        opencv_core
        opencv_imgcodecs
        opencv_imgproc
        ZLIB::ZLIB
)

# ---------------------------------------------------------------------
# Python extension
# ---------------------------------------------------------------------
pybind11_add_module(pijcodec src/bindings.cpp)
target_link_libraries(pijcodec PRIVATE pijcodec_static)


# add_executable(pij_viewer src/main_viewer.cpp "src/bindings.cpp")
# target_link_libraries(pij_viewer PRIVATE pijcodec)