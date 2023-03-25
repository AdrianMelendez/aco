// gcc -o p calculate_pi_threaded.c -lm -fopenmp
#include <stdio.h>
#include <math.h>
#include <omp.h>

void main(){
    omp_set_num_threads(omp_get_num_procs()); // Request as many threads as processors
    
    double sum=0.;
    int k=0;

    for (k=0; k<100; k++){
        sum = sum + pow((-1./3.), k) * 1./(2*k+1);
    }
    sum = sqrt(12)*sum;
    printf("The result for pi is: %E \n", sum);
}