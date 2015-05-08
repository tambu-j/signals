#include <limits>

class OnlineTvwap
{
public:
	OnlineTvwap(int capacity, double window) 
		: t_(nullptr),
          px_(nullptr),
          sz_(nullptr),
          sumSz_(0.),
          sumSzPx_(0.),
          tvwap_(std::numeric_limits<double>::quiet_NaN()),
          capacity_(capacity),
          window_(window),
          cur_(-1),
          refi_(0) 
	{
		initialiseBuffers();
	}

	~OnlineTvwap() 
	{
        if (t_ != nullptr)
	        delete t_;
	    if (px_ != nullptr)
            delete px_;
  	    if (sz_ != nullptr)
            delete sz_;
	}

    double add(double t, double price, double size)
    {
        // add a time, price and size to our cache
        //
        // this is where you'd need a sanity check on 
        // looping the buffer before you should (i.e.
        // if next element is 0 then t[0] should be 
        // less than t - window_ otherwise we've 
        // written over data we need)
        //
        // we assume t is positive and monotonically increasing, and don't bother checking...?
        //
        double tzero = t - window_;
        if ( (++cur_ % capacity_) == 0 ) {
            cur_ = 0;
            if (t_[0] >= tzero) {
                // re-allocate... must set cur back to the old capacity
            	cur_ = capacity_;
                reinitialiseBuffers();
            }
        }
        
        t_[cur_] = t;
        px_[cur_] = price;
        sz_[cur_] = size;
        
        // adjust the tvwap
        int tzeroi = refi_;
        
		// refi is the earliest item from the previous addition, so as t is
		// monotonically increasing we go "up" the buffer looking for the first t
		// inside our new bound [t-window_,t]; will always give us a solution (we
		// will hit cur_ eventually)
		while (t_[tzeroi] < tzero) {
		   tzeroi++;
		   if (tzeroi == cur_) // we've gone as fas as the current measurement
			   break;
		   if ((tzeroi % capacity_) == 0) // "round" the buffer
			   tzeroi = 0;
		}

		// now we must remove size weighted prices from the online sums,
		// again moving "up" the buffer from refi until it meets the new bound
		while (refi_ != tzeroi) {
		   sumSzPx_ -= (sz_[refi_] * px_[refi_]);
		   sumSz_ -= sz_[refi_];

		   if ((++refi_ % capacity_) == 0)
			   refi_ = 0;
		}

		// now add the new measurement
		sumSzPx_ += (sz_[cur_] * px_[cur_]);
		sumSz_ += sz_[cur_];

		tvwap_ = sumSz_ > 0. ? sumSzPx_ / sumSz_ : std::numeric_limits<double>::quiet_NaN();

		return tvwap_;
    }

private:

    void initialiseBuffers() {
	    t_ = new double[capacity_];
	    px_ = new double[capacity_];
        sz_ = new double[capacity_];

        for (int i = 0; i < capacity_; i++) {
            t_[i] = -2*window_;
            px_[i] = 0;
            sz_[i] = 0;
        }
    }

    void reinitialiseBuffers() {
    	// this is quite crude and assumes that it won't get called often (i.e. you'll size it up reasonably
    	// at initial construction), not least it'll most likely get called during a burst if you're not
    	// careful, and it'll be slow...

    	// don't lose the old capacity and data
    	int origCapacity = capacity_;
    	double* origT = t_;
    	double* origPx = px_;
    	double* origSz = sz_;

    	capacity_ *= 2;
    	t_ = new double[capacity_];
    	px_ = new double[capacity_];
    	sz_ = new double[capacity_];

    	// copy across the old data to the new buffers
    	for (int i = 0; i < capacity_; ++i) {
    		if (i < origCapacity) {
    			t_[i] = origT[i];
    			px_[i] = origPx[i];
    			sz_[i] = origSz[i];
    		}
    		else {
    			t_[i] = -2*window_;
    			px_[i] = 0;
    			sz_[i] = 0;
    		}
    	}

    	// delete the old data
    	delete [] origT;
    	delete [] origPx;
    	delete [] origSz;
    }

private:
    double* t_;
    double* px_;
    double* sz_;
    double sumSz_;
    double sumSzPx_;
    double tvwap_;
    int cur_;
    int refi_;
    int capacity_;
    double window_;
};
