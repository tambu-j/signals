__author__ = 'tim'

import numpy as np
from atj.tvwap import tvwap_trades, online_tvwap_trades

t = np.asarray([1.,2.,3.,4.,5.,6.,7.,8.,9.,10])
px = np.asarray([100.,101.,99.,100.,110.,111.,109.,110.,120.,121.])
sz = np.asarray([50.,25.,25.,50.,50.,75.,75.,10.,20.,30])

window = 5

tvres = tvwap_trades(window,t,px,sz)
otvres = online_tvwap_trades(window,t,px,sz)
tvres-otvres
np.any(tvres-otvres)