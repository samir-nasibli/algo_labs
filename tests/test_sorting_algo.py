import pytest
import numpy as np
from numpy.random import randint, seed

import algorithms


@pytest.mark.parametrize("sort_func",
                         ['bucket_argsort',
                          'quick_argsort',
                          'radix_argsort'],
                          ids=['bucket_argsort',
                              'quick_argsort',
                              'radix_argsort'])
def test_sorted(sort_func):
    size = 30
    # the same seed for all samples
    seed(1232)
    array = randint(1, 50, 30, dtype=np.uint64)
    res_ind = getattr(algorithms, sort_func)(array)
    prev = -1
    for i in res_ind:
        assert array[i] >= prev
        prev = array[i]
