// gcc -o p calculate_pi_time_diff.c -lm -fopenmp
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

void main(){
    
    double sum1=0., sum2=0;
    int k=0, NTERMS=1E8;
    clock_t begin, end;

    // SERIAL WAY
    begin = clock();
    for (k=0; k<NTERMS; k++){
        sum1 = sum1 + pow((-1./3.), k) * 1./(2*k+1);
    }
    sum1 = sqrt(12)*sum1;
    end = clock();
    double time = 1000.*(end - begin)/CLOCKS_PER_SEC; 
    printf("Time serial =%E ms \n",time);
    printf("The result for pi using serial is: %.20f \n", sum1);

    // THREADED WAY
    omp_set_num_threads(omp_get_num_procs()); // Request as many threads as processors
    double begin2 = omp_get_wtime();
    #pragma omp parallel shared(sum2) 
    {
    #pragma omp for reduction(+:sum2) 
        for (k=0; k<100; k++){
            sum2 = sum2 + pow((-1./3.), k) * 1./(2*k+1);
        }
    }
    sum2 = sqrt(12)*sum2;
    double time2 = 1000.*(omp_get_wtime() - begin2); //ms
    printf("Time threading=%E ms \n",time2);
    printf("The result for pi using threads is: %.20f \n", sum2);
}