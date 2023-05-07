#include <stdio.h>
#include <math.h>
// #include <omp.h>

#include "utility.h"

//*****************************************************************
//                          PARAMETERS
//*****************************************************************
#define NPART   101 //2001    // number of SPH particles
#define NSTEPS  500     // number of integration steps
#define NSPH    5 //25      // number of SPH neighbours (smaller tha NPART)
#define NOUT    100     // every NOUT step an output will be written

#define Tend    0.05    // integration end time

#define GAMMA   (7./5.) // adiabatic index

#define m       1.      // mass of each particle

//*****************************************************************
//                      PARTICLE STRUCTURE
//*****************************************************************
struct particle{

    double x;
    double v;
    double rho;
    double e;
    double h;
    
    double v_coeff;
    double rho_coeff;
    double e_coeff;

};

//*****************************************************************
//                          KERNEL
//*****************************************************************
double W(double r, double h){
    double ratio = r/h;
    if (ratio>=0.0 && ratio<=1.0){
        return 2.0/3.0 * h * (1.0 - 1.5 * pow2(ratio) + 0.75 * pow3(ratio));
    }
    else{
        if (ratio>=1.0 && ratio<=2.0){
            return 2.0/3.0 * h * (0.25 * pow3(2-ratio));
        }
        else{
            return 0.0;
        }
    }
}

// double W_coeff(int i, int j, struct particle particle_k){
//     return W()
// }

//*****************************************************************
//                      INITIAL CONDITIONS
//*****************************************************************
void initial_conditions(struct particle particles[]){
    double dx = 1./(NPART-1);
    for (int i=0; i<NPART;i++){
        particles[i].x = dx*i;
        particles[i].v = 0.;
        particles[i].e = 1e-5;
        particles[i].rho = 1/dx;
        if (i==(NPART-1)/2) particles[i].e=1;
    }
}

//*****************************************************************
//                      PERFORM INTEGRATION
//*****************************************************************
void performIntegration(struct particle particles[], double dt){
    for (int i=0; i<NPART;i++){
        particles[i].x      = particles[i].x    + particles[i].v*dt;
        particles[i].v      = particles[i].v    + particles[i].v_coeff*dt;
        particles[i].rho    = particles[i].rho  + particles[i].rho_coeff*dt;
        particles[i].e      = particles[i].e    + particles[i].e_coeff*dt;
    }
}

//*****************************************************************
//                       OUTPUT FILE
//*****************************************************************
void WriteOutputFile(struct particle particles[], double t){
    char filename[] = "time=";
    char str[100];
    sprintf(str,"%.3f.dat", t);
    strcat(filename,str);
    FILE *out_file = fopen(filename, "w");
    fprintf(out_file, "x,v,rho,e\n");
    for (int i=0; i<NPART;i++){
        fprintf(out_file,"%lf,%lf,%lf,%lf\n",particles[i].x,particles[i].v,particles[i].rho,particles[i].e);
    }
}

//*****************************************************************
//                         SET H
//*****************************************************************
void set_h(struct particle particles[]){
    for (int i=0; i<NPART;i++){
        double distances[NPART];
        double position_i = particles[i].x;
        for (int j=0; j<NPART;j++){
            distances[j] = abs(position_i - particles[j].x);
        }
    }
}

//*****************************************************************
//                          MAIN
//*****************************************************************
void main(){
    double t=0.;
    struct particle particles[NPART];
    initial_conditions(particles);

    for (int i=0; i<NSTEPS; i++){
        // set_h(particles);
        // getSPHcoefficient();
        double dt = Tend/NSTEPS;
        performIntegration(particles, dt);

        if ((i+1)%100==0) WriteOutputFile(particles, t);
    }

    for (int i=0; i<NPART;i++){
        fprintf(stderr,"%d: e=%lf\n", i, particles[i].e);
    }
}