#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define G 6.67E-11
#define M 1.989E30

double funcx(double y[]){
    // y[0] -> x
    // y[1] -> y
    // y[2] -> v_x
    // y[3] -> v_y
    double r = sqrt(y[0]*y[0] + y[1]*y[1]);
    return -G*M/pow(r,3)*y[0]; 
}

double funcy(double y[]){
    // y[0] -> x
    // y[1] -> y
    // y[2] -> v_x
    // y[3] -> v_y
    double r = sqrt(y[0]*y[0] + y[1]*y[1]);
    return -G*M/pow(r,3)*y[1];
}


double *leapfrog_step(double t,double y[], double dt){
    static double yres[4];
    double vx_adv, vy_adv;
    vx_adv = y[2] + funcx(y)*dt/2.0;
    yres[0] = y[0] + vx_adv*dt;
    vy_adv = y[3] + funcy(y)*dt/2.0;
    yres[1] = y[1] + vy_adv*dt;
    double y2[4]={yres[0],yres[1],y[2],y[3]};
    yres[2] = vx_adv + funcx(y2)*dt/2.0;
    yres[3] = vy_adv + funcy(y2)*dt/2.0;
    
    return yres;
}

int main(){
    int N_steps = 100;

    double t_0=0, t_f=365*10*24*3600, dt;
    dt = (t_f - t_0)/N_steps; // uniform time steps

    double y[4]={1.5e11,0,0,30e3};
    double t=t_0;

    // initial conditions
    // printf("Enter initial conditions\n");
    // printf("x0 = ");
    // scanf("%lf", &y[0]);
    // printf("y0 = ");
    // scanf("%lf", &y[1]);
    // printf("vx0 = ");
    // scanf("%lf", &y[2]);
    // printf("vy0 = ");
    // scanf("%lf", &y[3]);

    // double *yprima = f(t, y);
    // printf("%lf", *(yprima+1));

    FILE *out_file = fopen("leapfrog.dat", "w");
    for (int i=0; i<N_steps+1;i++){
        // fprintf(stderr, "x=%lf, y=%lf, vx=%lf, vy=%lf\n", y[0],y[1],y[2],y[3]);
        fprintf(out_file, "%lf,%lf,%lf\n", y[0],y[1],t);
        double *yres = leapfrog_step(t, y,dt);
        y[0] = *(yres);
        y[1] = *(yres+1);
        y[2] = *(yres+2);
        y[3] = *(yres+3);
        t = t + dt;
    }


    return 0;
}