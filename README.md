# PIJ Codec

## Overview
PIJ (short for "PNG inside JPEG") is a codec that enables embedding lossless regions of a PNG image within a lossy JPEG image. This is useful for applications that require preserving the quality of specific regions while benefiting from JPEG compression—such as face region encryption or scenarios where certain parts of the image need to remain lossless.

## Building the Project

### Steps to Build the Project
1. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```
2. Run CMake to configure the project:
   ```bash
   cmake -DCMAKE_TOOLCHAIN_FILE=C:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake -A x64 ..
   ```
3. Build the project using CMake:
   ```bash
    cmake --build . --config Release
    ```
    
