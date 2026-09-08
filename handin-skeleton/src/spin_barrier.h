#ifndef _SPIN_BARRIER_H
#define _SPIN_BARRIER_H

#include <pthread.h>
#include <iostream>
#include <atomic>
#include <thread>
using namespace std;

class spin_barrier {

    public:
        spin_barrier() = default;
        //void setCounter(int count);
        void loop();
        void reset();
        void getnumThreads(int num_threads) {
            numThreads = num_threads;
        }
    
    private:
        int numThreads;
        atomic<int> counter{0};
        atomic<int> go{0};
        //int counter;
};

#endif
