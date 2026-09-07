#include "prefix_sum.h"
#include "helpers.h"
using namespace std;


pthread_barrier_t barrier;
spin_barrier spinBarrier;
atomic<int> counter{0};

void barrier_init(int blocks) {
       pthread_barrier_init(&barrier, NULL, blocks);
}

void barrier_destroy() {
    pthread_barrier_destroy(&barrier);
}

void increment() {
    counter++;
}

void decrement() {
    counter--; 
}

void spinlock_wait() {
    increment();
    spinBarrier.setCounter(counter.load());
    spinBarrier.loop();
    decrement();
}

void* compute_prefix_sum(void *a)
{
    prefix_sum_args_t *args = (prefix_sum_args_t *)a;
    int n = args->n_vals;
    int* input = args->input_vals;
    int* output = args->output_vals;
    int n_loops = args->n_loops;
    int (*scan_operator)(int, int, int);
    scan_operator = args->op;
    int j;
    int blocks = args->n_threads;
    int i = args->thread_num;

    //spinBarrier.getnumThreads(blocks);
    //pthread_barrier_t barrier;
    //pthread_barrier_init(&barrier, NULL, blocks);

    for(j = i * n / (blocks); j < (i+1) * n / (blocks); j++) {
        //if (i == blocks - 1) {break;}
        if(j == i * n / (blocks)) {output[j] = input[j];}
        else {output[j] = scan_operator(input[j-1], input[j], n_loops);}
    }

    pthread_barrier_wait(&barrier);
    //spinlock_wait();


    int x = 0;
    int offset = 0;

    for(i = 1; i < blocks; i++) {
        //x += output[i*n/(blocks)-1];
        x = scan_operator(x, output[i*n/(blocks)-1], n_loops);
        output[i*n/(blocks)] = x;
    }
    pthread_barrier_wait(&barrier);
    //spinlock_wait();

    for(j = i * n / (blocks); j < (i+1) * n / (blocks); j++) {
        // stride = n / blocks
        //if (i==0) {break;}
        offset = output[i*n/(blocks)];
        output[j + (n/blocks)] = scan_operator(output[j + (n/blocks)], offset, n_loops);
    }

    pthread_barrier_wait(&barrier);
    //spinlock_wait();

   //pthread_barrier_destroy(&barrier);

    return 0;
}
