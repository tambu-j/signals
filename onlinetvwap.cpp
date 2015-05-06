#include "OnlineTvwap.h"

extern "C" void c_online_tvwap(double window, int datalen, double *t, double *px, double *sz, double *res)
{
    OnlineTvwap otvwap(100,window);
    for (int i = 0; i < datalen; ++i)
	   res[i] = otvwap.add(t[i],px[i],sz[i]); 
}
