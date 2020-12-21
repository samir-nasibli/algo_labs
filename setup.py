from distutils.core import setup, Extension
from Cython.Build import cythonize
import os
import numpy


if __name__ == '__main__':
    #os.environ["CC"] = "clang++"
    os.environ["CC"] = "gcc-10"
    #os.environ["CXX"] = "clang++"
    os.environ["CXX"] = "g++-10"
    #os.environ["CXX"] = "clang-10"
    algorithms_backend = Extension(
    name="algorithms",
    sources=["algorithms_ext.pyx", "src/sorting.cpp"],
    libraries=[],
    include_dirs=[numpy.get_include()],
    extra_compile_args=['-std=c++20'],
    define_macros=[("NPY_NO_DEPRECATED_API", "NPY_1_7_API_VERSION")],
    language="c++"
)

    setup (name = 'algorithms',
           version = '1.0',
           ext_modules = cythonize([algorithms_backend]),
           include_dirs=[numpy.get_include()])
