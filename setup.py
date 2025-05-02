from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension
import subprocess
import os

def get_opencv_flags():
    opencv_flags = subprocess.check_output(['pkg-config', '--cflags', '--libs', 'opencv4']).decode('utf-8').strip().split()
    include_dirs = []
    libraries = []
    library_dirs = []
    
    for flag in opencv_flags:
        if flag.startswith('-I'):
            include_dirs.append(flag[2:])
        elif flag.startswith('-l'):
            libraries.append(flag[2:])
        elif flag.startswith('-L'):
            library_dirs.append(flag[2:])
            
    return include_dirs, libraries, library_dirs

opencv_includes, opencv_libs, opencv_lib_dirs = get_opencv_flags()

# Add all source directories to include paths
src_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "src")
include_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "include")
include_dirs = opencv_includes + [src_dir, include_dir]

ext_modules = [
    Pybind11Extension(
        "pypij",
        ["src/main.cpp", "src/PIJCodec.cpp"],  # Add all source files here
        include_dirs=include_dirs,
        libraries=opencv_libs,
        library_dirs=opencv_lib_dirs,
        cxx_std=14
    ),
]

setup(
    name="pypij",
    ext_modules=ext_modules,
    setup_requires=["pybind11>=2.13.0"],
    install_requires=["pybind11>=2.13.0"],
)