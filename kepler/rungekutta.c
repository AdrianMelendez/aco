#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define G 6.67E-11
#define M 1.989E30

double *f(double t, double y[]){
    // y[0] -> x
    // y[1] -> y
    // y[2] -> v_x
    // y[3] -> v_y
    static double yprima[4];
    double r = sqrt(y[0]*y[0] + y[1]*y[1]);

    yprima[0] = y[2];
    yprima[1] = y[3];
    yprima[2] = -G*M/pow(r,3)*y[0];
    yprima[3] = -G*M/pow(r,3)*y[1];

    return yprima; //it is a pointer
}


double *rk4_step(double t,double y[], double dt, double *(*f)(double t, double y[])){
    double k1[4], k2[4], k3[4], k4[4];
    static double yres[4];
    
    for (int j=0; j<4; j++){
        k1[j] = dt * *(f(t,y)+j); 
    }

    double y2[]={y[0]+0.5*k1[0],y[1]+0.5*k1[1],y[2]+0.5*k1[2],y[3]+0.5*k1[3]};
    for (int j=0; j<4; j++){   
        k2[j] = dt * *(f(t+0.5*dt, y2)+j);
    }
        
    double y3[]={y[0]+0.5*k2[0],y[1]+0.5*k2[1],y[2]+0.5*k2[2],y[3]+0.5*k2[3]};
    for (int j=0; j<4; j++){
        k3[j] = dt * *(f(t+0.5*dt, y3)+j);
    }
        
    double y4[]={y[0]+k3[0],y[1]+k3[1],y[2]+k3[2],y[3]+k3[3]};
    for (int j=0; j<4; j++){
        k4[j] = dt * *(f(t+dt, y4)+j);
    }

    for (int j=0; j<4; j++){  
        yres[j] = y[j] + (k1[j] + 2*k2[j] + 2*k3[j] + k4[j]) / 6.0;
    }
    // fprintf(stderr, "k1=%lf, k2=%lf, k3=%lf, k4=%lf\n", k1,k2,k3,k4);
    // fprintf(stderr,"\n");
    return yres;
}

int main(){
    int N_steps = 48;

    double t_0=0, t_f=365*5*24*3600, dt;
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

    FILE *out_file = fopen("rk4.dat", "w");
    for (int i=0; i<N_steps+1;i++){
        // fprintf(stderr, "x=%lf, y=%lf, vx=%lf, vy=%lf\n", y[0],y[1],y[2],y[3]);
        fprintf(out_file, "%lf,%lf,%lf\n", y[0],y[1],t);
        double *yres = rk4_step(t, y,dt, *f);
        y[0] = *(yres);
        y[1] = *(yres+1);
        y[2] = *(yres+2);
        y[3] = *(yres+3);
        t = t + dt;
    }


    return 0;
}