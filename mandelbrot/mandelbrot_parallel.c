// gcc -o p mandelbrot_parallel.c -lm -fopenmp
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

int check_divergence(double c_real, double c_img, int NMAXITERATIONS){
    double z_real=0., z_img=0.;
    double sq_real, sq_img, module;

    z_real = c_real;
    z_img = c_img;

    // cannot be parallelised: each iteration needs the previous one
    for (int n=0; n<NMAXITERATIONS; n++){
        // printf("%E ---", z_real);
        // printf("%E\n", z_img);
        sq_real = z_real*z_real - z_img*z_img;
        sq_img = 2*z_real*z_img;
        z_real = sq_real + c_real;
        z_img = sq_img + c_img;

        if (z_real != z_real || z_img != z_img){ //check for NaN
            return 1; // divergence is true
        }
    }

    module = z_real*z_real - z_img*z_img;
    if (module > 2){
        return 1; // divergence is true
    }
    else{
        return 0; // divergence is false
    }

}

void output_file(double c_real[], double c_img[], int not_diverged[], int NPOINTS_PER_DIMENSION){
    FILE *out_file = fopen("mandelbrot_parallel.dat", "w");
    fprintf(out_file, "c_real,c_img,divergence\n");

    double start_output = omp_get_wtime();
    // cannot be parallelised to assure the data is propperly written
    for (int i=0; i<NPOINTS_PER_DIMENSION; i++){
        for (int j=0; j<NPOINTS_PER_DIMENSION; j++){
            fprintf(out_file, "%.6f,%.6f,%d\n",c_real[i],c_img[j],not_diverged[j+i*NPOINTS_PER_DIMENSION]);
        }
    }
    double time_output = (omp_get_wtime() - start_output); //s
    printf("Time output =%E s \n",time_output);

}

void main(){

    double *c_real, *c_img;
    int *not_diverged;
    int NMAXITERATIONS=100, NPOINTS_PER_DIMENSION=5000;

    double c_real_min=-2., c_real_max=0.5; // limits given by the exercise
    double c_img_min=-1.1, c_img_max=1.1;
    double step_real, step_img;

    step_real = (c_real_max - c_real_min) / (NPOINTS_PER_DIMENSION-1);
    step_img  = (c_img_max -c_img_min) / (NPOINTS_PER_DIMENSION-1);

    c_real = calloc(NPOINTS_PER_DIMENSION, sizeof(double)); //allocate doubles
    c_img = calloc(NPOINTS_PER_DIMENSION, sizeof(double));
    not_diverged = calloc(NPOINTS_PER_DIMENSION*NPOINTS_PER_DIMENSION, sizeof(int));

    double start = omp_get_wtime();
    omp_set_num_threads(omp_get_num_procs()); // Request as many threads as processors
    printf("num_proc=%d \n",omp_get_num_procs());

    #pragma omp parallel
    {
    
    
    for (int i=0; i<NPOINTS_PER_DIMENSION; i++){
        c_real[i] = c_real_min + step_real * i;

        #pragma omp for // faster parallelising the innerloop
        for (int j=0; j<NPOINTS_PER_DIMENSION; j++){
            c_img[j] = c_img_min + step_img * j;
            not_diverged[j+i*NPOINTS_PER_DIMENSION] = check_divergence(c_real[i], c_img[j], NMAXITERATIONS);
            // printf("%d\n", check_divergence(c_real[i], c_img[j], NMAXITERATIONS));
        }
    }
    }
   output_file(c_real, c_img, not_diverged, NPOINTS_PER_DIMENSION);
    

   double time = (omp_get_wtime() - start); //s
   printf("Time omp =%E s \n",time);  
   
   // NPOINTS_PER_DIMENSION=1000: ~1.17 s
   // NPOINTS_PER_DIMENSION=5000: 
   // -1 thread: ~32 s
   // -6 threads: ~ 26 s 
}