#include <stdio.h>
#include <math.h>

#define G 1

double func(double M, double x, double y){
    double r = sqrt(x*x + y*y);
    return -G*M/pow(r,3);
}

double leapfrog(double x0,double y0,double v_x0, double v_y0, double M){
    int N_steps = 1000;

    double t_0=0, t_f=300, dt;
    dt = (t_f - t_0)/N_steps;

    double *x, *y, *v_x, *v_y;

    x = calloc(N_steps, sizeof(double)); //allocate doubles
    y = calloc(N_steps, sizeof(double));
    v_x = calloc(N_steps, sizeof(double)); 
    v_y = calloc(N_steps, sizeof(double)); 

    // initial conditions
    x[0] = x0;
    y[0] = y0;
    v_x[0] = v_x0;

    // advance velocity
    v_x[0] = v_x0 + dt/2*func(M,x0,y0)*x0; //velocity arrays shifted dt/2 to position arrays
    v_y[0] = v_x0 + dt/2*func(M,x0,y0)*y0;

    for (int i=0; i<N_steps;i++){

    }

}