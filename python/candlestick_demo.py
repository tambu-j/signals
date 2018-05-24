import numpy as np
import matplotlib.pyplot as plt
from matplotlib.finance import candlestick2_ochl

import atj.candlestick

vsamplet = np.asarray([1., 2., 3., 4., 5., 6.])
lookback = 2
vt = np.asarray([1., 2., 3., 4., 5.])
vpx = np.asarray([108., 102., 103., 101., 105.])
vsz = np.asarray([10., 15., 5., 25., 20.])
res = atj.candlestick.candlestick_trades(vsamplet, lookback, vt, vpx, vsz)
res = np.reshape(res, (-1, 6))

fig, ax = plt.subplots()
#fig.subplots_adjust(bottom=0.2)
#ax.xaxis.set_major_locator(mondays)
#ax.xaxis.set_minor_locator(alldays)
#ax.xaxis.set_major_formatter(weekFormatter)
##ax.xaxis.set_minor_formatter(dayFormatter)

np.save
candlestick2_ochl(ax, res[:, 1], res[:, 2], res[:, 3], res[:, 4], width=0.2, colorup=u'k', colordown=u'r', alpha=1.0)

##plot_day_summary(ax, quotes, ticksize=3)

ax.xaxis_date()
ax.autoscale_view()
plt.setp(plt.gca().get_xticklabels(), rotation=45, horizontalalignment='right')

plt.show()

