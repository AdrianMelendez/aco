#include <stdio.h>
#include <string.h>

//create struct with person1 variable
typedef struct Person{
    char name[50];
    int SNo;
    float height;
} person;

void main(){
    person person1;
    strcpy(person1.name, "George Orwell");
    person1.SNo = 1984;
    person1.height=3.5;
    printf("height= %E", person1.height);
}