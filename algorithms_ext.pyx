cimport numpy as cnp
import numpy as np
cimport cython


ctypedef unsigned long uint64_t


cdef extern from "src/sorting.cpp" namespace "sort_algorithms":
    void backend_bucket_argsort(uint64_t * begin, uint64_t * result, size_t size)
    void backend_quick_argsort(uint64_t * begin, uint64_t * result, size_t size)
    void backend_radix_argsort(uint64_t * begin, uint64_t * result, size_t size)


def bucket_argsort(cnp.ndarray array):
    """
    bucket_argsort(np.ndarray array)

    Returns indicies of given array sorted by bucket sort algorithm.

    Parameters
    ----------
    array : ndarray
        Array to be sorted. Should be C-contigous.

    Returns
    -------
    sorted_indicies : ndarray
        Array of the indicies of sorted ``array``.

    Note
    ----
    ``array`` is supported as `uint64` dtype.

    """

    #if array.ndim != 1:
    #    raise ValueError("Given array should be 1 dim.")
    if array.dtype != np.uint64:
        raise TypeError("Only np.uint64 supported.")

    indicies_arr = np.zeros_like(array, dtype=np.uint64)

    cdef size_t size = len(array)
    cdef uint64_t * array_c
    cdef uint64_t * indicies_arr_c
    array_c = <uint64_t *>cnp.PyArray_DATA(array)
    indicies_arr_c = <uint64_t *>cnp.PyArray_DATA(indicies_arr)
    backend_bucket_argsort(array_c, indicies_arr_c, size)

    return indicies_arr


def quick_argsort(cnp.ndarray array):
    """
    bucket_argsort(np.ndarray array)

    Returns indicies of given array sorted by quick sort algorithm.

    Parameters
    ----------
    array : ndarray
        Array to be sorted. Should be C-contigous.

    Returns
    -------
    sorted_indicies : ndarray
        Array of the indicies of sorted ``array``.

    Note
    ----
    ``array`` is supported as `uint64` dtype.

    """

    #if array.ndim != 1:
    #    raise ValueError("Given array should be 1 dim.")
    if array.dtype != np.uint64:
        raise TypeError("Only np.uint64 supported.")

    indicies_arr = np.zeros_like(array, dtype=np.uint64)

    cdef size_t size = len(array)
    cdef uint64_t * array_c
    cdef uint64_t * indicies_arr_c
    array_c = <uint64_t *>cnp.PyArray_DATA(array)
    indicies_arr_c = <uint64_t *>cnp.PyArray_DATA(indicies_arr)
    backend_quick_argsort(array_c, indicies_arr_c, size)

    return indicies_arr


def radix_argsort(cnp.ndarray array):
    """
    radix_argsort(np.ndarray array)

    Returns indicies of given array sorted by radix sort algorithm.

    Parameters
    ----------
    array : ndarray
        Array to be sorted. Should be C-contigous.

    Returns
    -------
    sorted_indicies : ndarray
        Array of the indicies of sorted ``array``.

    Note
    ----
    ``array`` is supported as `uint64` dtype.

    """

    #if array.ndim != 1:
    #    raise ValueError("Given array should be 1 dim.")
    if array.dtype != np.uint64:
        raise TypeError("Only np.uint64 supported.")

    indicies_arr = np.zeros_like(array, dtype=np.uint64)

    cdef size_t size = len(array)
    cdef uint64_t * array_c
    cdef uint64_t * indicies_arr_c
    array_c = <uint64_t *>cnp.PyArray_DATA(array)
    indicies_arr_c = <uint64_t *>cnp.PyArray_DATA(indicies_arr)
    backend_radix_argsort(array_c, indicies_arr_c, size)

    return indicies_arr
