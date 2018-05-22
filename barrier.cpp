#include <cmath>
#include <limits>
#include <vector>

void barrier(int len, double *vals, double rbarrier, int maxlook, double *idx, double *diff)
{
    #pragma omp parallel for
    for (int i = 0; i < len; ++i) {
        double aidx = std::numeric_limits<double >::quiet_NaN();
        double adiff = std::numeric_limits<double >::quiet_NaN();
	double tmpdiff = 0.;
        if (!(vals[i] != vals[i])) {
            aidx = std::numeric_limits<double >::quiet_NaN();
	    adiff = std::numeric_limits<double >::quiet_NaN();
            for (int j = 1; j < maxlook + 1; ++j) {
                tmpdiff = vals[i+j] - vals[i];
                if (std::abs(adiff) >= rbarrier) {
                    aidx = j;
		    adiff = tmpdiff;
                }
            }
	}
        idx[i] = aidx;
        diff[i] = adiff;
    }
}

extern "C" void c_barrier(int len, double *vals, int rbarrier, int maxlook, double *idx, double *diff)
{
    barrier(len, vals, rbarrier, maxlook, idx, diff);
}
