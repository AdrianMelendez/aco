// gcc -o p sum_with_openmp.c -lm -fopenmp
#include <stdio.h>
#include <omp.h>
#include <math.h>

void main(){
    omp_set_num_threads(omp_get_num_procs()); // Request as many threads as processors
    
    double sum=0.;
    int k=0, NTERMS=1E5;

    #pragma omp parallel shared(sum) 
    {

    #pragma omp for reduction(+:sum) 
        for (k=0; k<NTERMS; k++){
            sum = sum + k;
        }
    }
        printf("The result is: %E \n", sum);
}