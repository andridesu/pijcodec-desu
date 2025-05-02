# PIJ Codec

## Overview
PIJ (short for "PNG inside JPEG") is a codec that enables embedding lossless regions of a PNG image within a lossy JPEG image. This is useful for applications that require preserving the quality of specific regions while benefiting from JPEG compression—such as face region encryption or scenarios where certain parts of the image need to remain lossless.

## Building the Project

### Prerequisites
- Cmake: install -> `sudo apt install cmake`
- g++: install -> `sudo apt install g++`
- libopencv-dev: install -> `sudo apt install libopencv-dev`
- nlohmann/json: install -> `sudo apt install nlohmann-json3-dev`

### Steps to Build the Project
1. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```
2. Run CMake to configure the project:
   ```bash
   cmake ..
   ```
3. Build the project using CMake:
   ```bash
    make
    ```

mkdir build && cd build && cmake .. && make

pip install pybind11