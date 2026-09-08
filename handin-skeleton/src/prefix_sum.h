#pragma once

#include <stdlib.h>
#include <pthread.h>
#include <spin_barrier.h>
#include <iostream>
#include <pthread_barrier.h>
using namespace std;

void* compute_prefix_sum(void* a);
void barrier_init(int blocks);
extern pthread_barrier_t barrier;
extern spin_barrier spinBarrier;
void barrier_init(int blocks);
void barrier_destroy();