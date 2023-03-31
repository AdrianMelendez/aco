#include <stdio.h>

void main(int argc, char **argv)
{
    printf("argc = %d\n", argc);
    printf("argv = %s\n", argv[1]);
    printf("argv = %d\n", atoi(argv[1]));
}