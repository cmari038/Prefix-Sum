#include "prefix_sum.h"
#include "helpers.h"
using namespace std;


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
    int i;
    int j;
    int stride = 0;

    // Belloch

   /* for (i = 0; i < log2(n)-1; i++) {
        for(j = 0; j < n - 1; j += pow(2,i+1)) {
            input[j + exponent(2,i+1) - 1] = input[j + exponent(2,i) - 1] + input[j + exponent(2,i) + 1 - 1];
        }
    }

    input[n-1] = 0;
    int holder = 0;

    for(i = log2(n)-1; i >= 0; i--) {
        for(j = 0; j < n - 1; j += pow(2,i+1)) {
            holder = input[j + exponent(2,i+1) - 1];
            input[j + exponent(2,i+1) - 1] = input[j + exponent(2,i) + 1 - 1];
            input[j + exponent(2,i) + 1 - 1] += holder; 
        }
    } */

    // two-stage partition

    int blocks = args->n_threads;
    int sums[n]; 

    for(i = 0; i < blocks; i++) {
        for(j = i * n / (blocks+1); j < (i+1) * n / (blocks+1); j++) {
            output[j] = scan_operator(input[i-1], input[i], n_loops);
            // syncthreads
        }
    }

    int x = 0;

    for(i = 0; i < blocks; i++) {
        x += output[i*n/(blocks+1)-1];
        output[i*n/(blocks+1)-1] = x;
    }

    for(i = 0; i < blocks; i++) {
        for(j = i * n / (blocks+1); j < (i+1) * n / (blocks+1); j++) {
            int offset = input[i*n/(blocks+1)];
            output[j] = offset;
        }
        //syncthreads
    }


    /************************
     * Your code here...    *
     * or wherever you like *
     ************************/

    return 0;
}
