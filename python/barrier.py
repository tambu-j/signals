import numpy as np
import ctypes


def _getpath():
    return r"/home/tim/dev/signals/build"


def _load_signals_lib():
    return np.ctypeslib.load_library("libsignals", _getpath())


def barrier(vals, rbarrier, maxlen):
    #requires = ["CONTIGUOUS", "ALIGNED"]

    lib = _load_signals_lib()
    lib.c_barrier.restype = None
    lib.c_barrier.argtypes = [np.ctypeslib.c_intp,
                              np.ctypeslib.ndpointer(float, ndim=1,
                                                     flags="aligned, contiguous"),
                              ctypes.c_double,
                              np.ctypeslib.c_intp,
                              np.ctypeslib.ndpointer(float, ndim=1,
                                                     flags="aligned, contiguous"),
                              np.ctypeslib.ndpointer(float, ndim=1,
                                                     flags="aligned, contiguous,"
                                                           "writeable")]

    datalen = np.alen(vals)
    res_idx = np.empty(datalen)
    res_rdiff = np.empty(datalen)
    lib.c_barrier(datalen, vals, rbarrier, maxlen, res_idx, res_rdiff)
    return res_idx, res_rdiff


sample = np.random.normal(0, 2, 200000).cumsum()
a, b = barrier(sample, 10, 100)
