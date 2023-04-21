#include <stdio.h>
#include <string.h>


struct Person{
    char name[50];
    int age;
    float height;
} ;

void main(){
    struct Person *personPtr, person1;
    personPtr = &person1;
    person1.age = 10;
    printf("Age = %d\n", person1.age);
    printf("Age = %d\n", personPtr->age);
}