#include <stdio.h>
int main(){
    int x[4]={1,2,3,4};
    int i;

    for (i=0;i<4;++i){
        printf("&x[%d] = %p, %p\n", i, &x[i], x+i);
        printf("x[%d] = %d, %d\n", i, x[i], *(x+i));
    }
    return 0;
}