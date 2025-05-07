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

# your Python extension
pybind11_add_module(pypij
    src/main.cpp
    src/PIJCodec.cpp
)
target_link_libraries(pypij PRIVATE ${OpenCV_LIBS} ${ZLIB_LIBRARIES})

# new: standalone viewer
add_executable(pij_viewer
    src/pij_viewer.cpp
    src/PIJCodec.cpp      # reuse the same decode implementation
)
target_link_libraries(pij_viewer
    PRIVATE
      ${OpenCV_LIBS}
      ${ZLIB_LIBRARIES}
)
