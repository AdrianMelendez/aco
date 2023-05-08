#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
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
//                     GET SPH COEFFICIENT
//*****************************************************************
double W(double r, double h){
    double ratio = r/h;
    if (ratio>=0.0 && ratio<=1.0){
        return 2.0/(3.0 * h) * (1.0 - 1.5 * pow2(ratio) + 0.75 * pow3(ratio));
    }
    else{
        if (ratio>=1.0 && ratio<=2.0){
            return 2.0/(3.0 * h) * (0.25 * pow3(2-ratio));
        }
        else{
            return 0.0;
        }
    }
}

double dW_ij(int i, int j, struct particle particles[]){
    double r = particles[i].x - particles[j].x;
    double abs_factor;
    if (r>0) {abs_factor=1.;}
    if (r<0) {r=-r; abs_factor=-1;}
    double h = particles[i].h;
    double ratio = r/h;
    if (ratio>=0.0 && ratio<=1.0){
        return abs_factor * 2.0 / h * ( - r / (pow2(h)) + 3.0/4.0 * pow2(r)/(pow3(h)));
    }
    else{
        if (ratio>=1.0 && ratio<=2.0){
            return abs_factor * -0.5 / (pow2(h)) * (pow2(2-ratio));
        }
        else{
            return 0.0;
        }
    }
}

double P_i(int i, struct particle particles[]){
    return (GAMMA-1) * particles[i].rho * particles[i].e;
}

void getSPHcoefficients(struct particle particles[]){
    for (int i=0; i<NPART;i++){
        double sum_v=0., sum_rho=0., sum_e=0.;
        for (int j=0; j>NPART;j++){
            if (j==i) continue;
            sum_v = sum_v + m * (P_i(j, particles)/pow2(particles[j].rho) + P_i(i, particles)/pow2(particles[i].rho)) * dW_ij(i,j,particles);
            sum_rho = sum_rho + m * (particles[i].v - particles[j].v) * dW_ij(i,j,particles);
            sum_e = sum_e + m * (P_i(j, particles)/pow2(particles[j].rho) + P_i(i, particles)/pow2(particles[i].rho)) * (particles[i].v - particles[j].v) * dW_ij(i,j,particles);
        }
        particles[i].v_coeff = -sum_v;
        particles[i].rho_coeff = sum_rho;
        particles[i].e_coeff = 0.5 * sum_e;
    }
}


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
            distances[j] = fabs(position_i - particles[j].x);
        }
        long sorted_index[NPART]={0};
        fprintf(stderr,"Array of distances:\n");
        indexx(NPART, distances, sorted_index);
        for (int j=0; j<NPART;j++){
        fprintf(stderr,"%lf,", distances[j]);
        }
        fprintf(stderr, "----\n\n");
        fprintf(stderr,"Array of sorted indexes:\n");
        for (int j=0; j<NPART;j++){
        fprintf(stderr,"%ld,", sorted_index[j]);
        }
        fprintf(stderr, "\n");
        particles[i].h = distances[sorted_index[NSPH]-1]; // notice the closest is the same particle
        break;
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
        set_h(particles);
        getSPHcoefficients(particles);
        double dt = Tend/NSTEPS;
        performIntegration(particles, dt);

        if ((i+1)%NOUT==0) WriteOutputFile(particles, t);
        t = t + dt;
    }

    // for (int i=0; i<NPART;i++){
    //     fprintf(stderr,"%d: e=%lf\n", i, particles[i].e);
    // }
}