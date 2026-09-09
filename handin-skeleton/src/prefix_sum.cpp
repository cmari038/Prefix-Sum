#include "prefix_sum.h"
#include "helpers.h"
using namespace std;


pthread_barrier_t barrier;
spin_barrier spinBarrier;

void barrier_init(int blocks) {
       pthread_barrier_init(&barrier, NULL, blocks);
}

void barrier_destroy() {
    pthread_barrier_destroy(&barrier);
}

void barrier_wait(bool spinlock) {
    if (spinlock) {
        spinBarrier.loop();
    }
    else {
        pthread_barrier_wait(&barrier);
    }
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
    bool spin = args->spin;
    int blocks = args->n_threads;
    int i = args->thread_num;

    for(j = i * n / (blocks); j < (i+1) * n / (blocks); j++) {
        //if (i == blocks - 1) {break;}
        if(j == i * n / (blocks)) {output[j] = input[j];}
        else {output[j] = scan_operator(input[j-1], input[j], n_loops);}
    }

   barrier_wait(spin);


    int x = 0;
    int offset = 0;

    for(int k = 1; k < blocks; k++) {
        if(i > 0) {break;}
        //x += output[i*n/(blocks)-1];
        x = scan_operator(x, output[k*n/(blocks)-1], n_loops);
        output[k*n/(blocks)] = x;
    }
    barrier_wait(spin);

    for(j = i * n / (blocks); j < (i+1) * n / (blocks); j++) {
        // stride = n / blocks
        //if (i==0) {break;}
        offset = output[i*n/(blocks)];
        output[j] = scan_operator(output[j], offset, n_loops);
    }

    barrier_wait(spin);

    return 0;
}
