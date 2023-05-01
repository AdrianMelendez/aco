// gcc -o p sort.c
#include <stdio.h>

// int sort(int *array){
//     int *sorted_indexes;
//     int max=array[0], max_index=0;

//     sorted_indexes = (int *) calloc(sizeof(array),sizeof(int));

//     for (int i=1;i<sizeof(array);i++){
//         if (array[i]>max && max_index!=){
//             max = array[i];
//             max_index=i;
//         }
//         else if(){

//         }

//     }
// }

int main(){
    unsigned char buffer[10];
    FILE *ptr;

    ptr = fopen("random_int_numbers.dat","rb");  // r for read, b for binary

    fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer
    for (int i=0; i<sizeof(buffer);i++) {
        fprintf(stderr, "%d\n", buffer[i]);
    } 
}
