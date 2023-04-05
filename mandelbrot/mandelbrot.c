// gcc -o p mandelbrot.c -lm 
#include <stdio.h>
#include <math.h>
// #include <omp.h>
#include <time.h>

int check_divergence(c_real, c_img, NMAXITERATIONS){
    double z_real = 0., z_img=0.;
    double sq_real, sq_img, module;

    for (int n=0; n<NMAXITERATIONS; N++){
        sq_real = z_real*z_real - z_img*z_img;
        sq_img = 2*z_real*z_img;
        z_real = sq_real + c_real;
        z_img = sq_img + c_img;
    }

    module = z_real*z_real - z_img*z_img;
    if (module > 2){
        return 1 // divergence is true
    }
    else{
        return 0 // divergence is false
    }

}

void main(){

    double *c_real, *c_img;
    int *not_diverged;
    int NMAXITERATIONS=100, NPOINTS_PER_DIMENSION=100;

    printf(check_divergence(0.,0.,100));
}