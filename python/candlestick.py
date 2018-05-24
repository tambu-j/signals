# __all__ = ['candlestick_trades']

import numpy as np
import ctypes
import os


def _getpath():
    #return r"/Users/tim/Library/Caches/clion10/cmake/generated/c57b29e0/c57b29e0/Debug/"
    return r"/Users/tim/dev/signals/build"


def _load_candlestick_lib():
    return np.ctypeslib.load_library("libsignals", _getpath())


def candlestick_trades(samplet, lookback, t, px, sz):
    #requires = ["CONTIGUOUS", "ALIGNED"]

    lib = _load_candlestick_lib()
    lib.c_candlestick.restype = None
    lib.c_candlestick.argtypes = [np.ctypeslib.c_intp,
                                  np.ctypeslib.ndpointer(float,
                                                         flags="aligned, contiguous"),
                                  ctypes.c_double,
                                  np.ctypeslib.c_intp,
                                  np.ctypeslib.ndpointer(float, ndim=1,
                                                         flags="aligned, contiguous"),
                                  np.ctypeslib.ndpointer(float, ndim=1,
                                                         flags="aligned, contiguous"),
                                  np.ctypeslib.ndpointer(float, ndim=1,
                                                         flags="aligned, contiguous"),
                                  np.ctypeslib.ndpointer(float, ndim=1,
                                                         flags="aligned, contiguous,"
                                                               "writeable")]

    # samplet = np.require(samplet, float, requires)
    # c = np.empty_like(a)
    samplelen = np.alen(samplet)
    datalen = np.alen(t)
    res = np.empty(6*samplelen)
    lib.c_candlestick(samplelen, samplet, lookback, datalen, t, px, sz, res)
    return res