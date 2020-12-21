# Getting started
Package for university labs use.

# Building `algorithms` using conda-build
The easiest way to build `algorithms` is using the conda-build with the provided recipe

## Prerequisites
* Python version >= 3.8
* conda-build version >= 3
* g++-9 or g++-10 compilers


## Build and install
### Building
```bash
cd <checkout-dir>
conda build conda-recipe

```
This will build the conda package and tell you where to find it (```.../algorithms*.tar.bz2```).

### Installing the built `algorithms` conda package
```bash
conda install <path-to-conda-package-as-built-above>
```
To actually use your `algorithms`, dependent packages need to be installed. To ensure, do

Linux:
```
conda install numpy
```

## Testing

```bash
pytest -vv
```
```

tests/test_sorting_algo.py::test_sorted[bucket_argsort] PASSED            [ 33%]
tests/test_sorting_algo.py::test_sorted[quick_argsort] PASSED             [ 66%]
tests/test_sorting_algo.py::test_sorted[radix_argsort] PASSED             [100%]

============================== 3 passed in 0.25s ===============================
```
