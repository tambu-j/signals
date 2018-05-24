#include "onlinetvwap.h"

#include <vector>

extern "C" void c_online_tvwap(double window, int datalen, double *t, double *px, double *sz, double *res)
{
    OnlineTvwap otvwap(2,window);
    for (int i = 0; i < datalen; ++i)
	   res[i] = otvwap.add(t[i],px[i],sz[i]); 
}

void tvwap(double window, int datalen, double *t, double *px, double *sz, std::vector<double>& results)
{
	results.resize(datalen);

	//#pragma omp parallel for
	for (int i = 0; i < datalen; ++i) {
		double tzero = t[i] - window;
		double sumSzPx = 0;
		double sumSz = 0;
		int j = i;
		while ((t[j] >= tzero) && (j >= 0)) {
			sumSzPx += sz[j] * px[j];
			sumSz += sz[j];
			j--;
		}
		results[i] = sumSz > 0 ? sumSzPx/sumSz : std::numeric_limits<double>::quiet_NaN();
	}
}

extern "C" void c_tvwap(double window,int datalen, double *t, double *px, double *sz, double *res)
{
    std::vector<double> vres;
    tvwap(window,datalen,t,px,sz,vres);
    for(int i = 0; i < vres.size(); ++i) {
        res[i] = vres[i];
    }
}
