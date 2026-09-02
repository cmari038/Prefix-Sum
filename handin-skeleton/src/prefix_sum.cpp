#include "prefix_sum.h"
#include "helpers.h"
using namespace std;


void block_sum(int block, int n_loops, int (*scan_operator)(int, int, int), int* output, int* input, int n_threads, int n) {
    int i = block;
    int blocks = n_threads;
    for(int j = i * n / (blocks+1); j < (i+1) * n / (blocks+1); j++) {
            output[j] = scan_operator(input[i-1], input[i], n_loops);
        }
}

void offset_sum(int block, int n_loops, int (*scan_operator)(int, int, int), int* output, int* input, int n_threads, int n) {
    int i = block;
    int blocks = n_threads;
    for(i = 1; i <= blocks; i++) {
        for(int j = i * n / (blocks+1); j < (i+1) * n / (blocks+1); j++) {
            int offset = input[i*n/(blocks+1)];
            output[j] += offset;
        }
    }
}
void* compute_prefix_sum(void *a)
{
    prefix_sum_args_t *args = (prefix_sum_args_t *)a;

    int id = args->t_id;
    int n = args->n_vals;
    int* input = args->input_vals;
    int* output = args->output_vals;
    int n_loops = args->n_loops;
    int (*scan_operator)(int, int, int);
    scan_operator = args->op;
    int j;
    int stride = 0;
    int blocks = args->n_threads;
    int i = args->thread_num;


    //for(i = 0; i < blocks; i++) {
    for(j = i * n / (blocks+1); j < (i+1) * n / (blocks+1); j++) {
        if (i == blocks) {break;}
        output[j] = scan_operator(input[i-1], input[i], n_loops);
    }
    //}

    int x = 0;

    for(i = 1; i <= blocks; i++) {
        x += output[i*n/(blocks+1)-1];
        output[i*n/(blocks+1)-1] = x;
    }

    //for(i = 1; i <= blocks; i++) {
    for(j = i * n / (blocks+1); j < (i+1) * n / (blocks+1); j++) {
        if (i==0) {break;}
        int offset = output[i*n/(blocks+1)];
        output[j] += offset;
    }
    //}


    /************************
     * Your code here...    *
     * or wherever you like *
     ************************/

    return 0;
}
