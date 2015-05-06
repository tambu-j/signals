#include <vector>

void candlestick(int samplelen, double *samplet, double lookback, int datalen, double *t, double *px, double *sz, std::vector<double>& results)
{
    // open px, close px, max px, min px, vwap

    // first build out the indexes we window, these are in pairs stored in a flat 1d array, so we would have:
    //  lookback = 30
    //  sampletimes = 830, 900, 930, 1000, ...
    //  t = 753, 805, 812, 829, 832, 848, 907, 930
    //  --> idx = [1,3,4,5,6,6,7,7]
    // Never mind for now --> more direct approach.

    // pre-seek the start t for each samplet
    std::vector<int> startTIdx;
    startTIdx.resize(samplelen);
    int hint = 0;
    for (int i = 0; i < samplelen; ++i) {
        double ltime = samplet[i] - lookback;
        int j = hint;
        while (j < datalen && ltime > t[j]) {
            j++;
        }
        startTIdx[i] = hint = j;
    }

    const int nout = 6;
    results.resize(nout*samplelen);
    #pragma omp parallel for
    for (int i = 0; i < samplelen; ++i) {
        double open = std::numeric_limits<double >::quiet_NaN();
        double close = std::numeric_limits<double >::quiet_NaN();
        double high = std::numeric_limits<double >::quiet_NaN();
        double low = std::numeric_limits<double >::quiet_NaN();
        double volume = std::numeric_limits<double >::quiet_NaN();
        double vwap = std::numeric_limits<double >::quiet_NaN();
        bool inited = false;
        double vwapNum = 0.;
        double vwapDen = 0.;
        int j = startTIdx[i];
        for( ; j < datalen && t[j] < samplet[i]; ++j) {
            if (inited) { // could take this branch out if it is too slow
                high = px[j] > high ? px[j] : high;
                low = px[j] < low ? px[j] : low;
                vwapNum += px[j] * sz[j];
                vwapDen += sz[j];
            }
            else {
		// open should be the price at the time of the start of the window
                if (t[j] == (samplet[i]-lookback))
			open = px[j];
		else if ((t[j] > (samplet[i]-lookback)) && j > 0) {
			open = px[j-1];
		}
		high = low = px[j];
                vwapNum = px[j] * sz[j];
                vwapDen = sz[j];
                inited = true;
            }
        }

        if (j > 0 && j < datalen)
            close = px[j-1];
        else if (j == datalen)
            if (t[datalen-1] >= (samplet[i]-lookback))
                close = px[datalen-1];

        vwap = vwapNum > 0. ? vwapNum/vwapDen : vwap;
	volume = vwapDen;

        results[i*nout] =  open;
        results[i*nout + 1] = close;
        results[i*nout + 2] = high;
        results[i*nout + 3] = low;
        results[i*nout + 4] = volume;
        results[i*nout + 5] = vwap;
    }
}

extern "C" void c_candlestick(int samplelen, double *samplet, double lookback,
                              int datalen, double *t, double *px, double *sz, double *res)
{
    std::vector<double> vres;
    candlestick(samplelen,samplet,lookback,datalen,t,px,sz,vres);
    for(int i = 0; i < vres.size(); ++i) {
        res[i] = vres[i];
    }
}
