# __all__ = ['tvwap_trades','online_tvwap_trades']

import numpy as np
import ctypes


def _getpath():
    return r"~/dev/signals/build"


def _load_signals_lib():
    return np.ctypeslib.load_library("libsignals", _getpath())


def tvwap_trades(window, t, px, sz):
    #requires = ["CONTIGUOUS", "ALIGNED"]

    lib = _load_signals_lib()
    lib.c_tvwap.restype = None
    lib.c_tvwap.argtypes = [ctypes.c_double,
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

    datalen = np.alen(t)
    res = np.empty(datalen)
    lib.c_tvwap(window, datalen, t, px, sz, res)
    return res

def online_tvwap_trades(window, t, px, sz):
    #requires = ["CONTIGUOUS", "ALIGNED"]

    lib = _load_signals_lib()
    lib.c_online_tvwap.restype = None
    lib.c_online_tvwap.argtypes = [ctypes.c_double,
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

    datalen = np.alen(t)
    res = np.empty(datalen)
    lib.c_online_tvwap(window, datalen, t, px, sz, res)
    return res
