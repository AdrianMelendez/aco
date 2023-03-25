// gcc -o p calculate_pi.c -lm 
#include <stdio.h>
#include <math.h>

void main(){
    double sum=0.;
    int k=0;

    for (k=0; k=10; k++){
        sum = sum + pow((-1./3.), k) * 1./(2*k+1);
    }
    sum = sqrt(12)*sum;
    printf("The result for pi is: %E", sum);
}