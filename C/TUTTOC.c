#include <stdio.h>
#include <stdbool.h>
int main() {
    printf("printf is to print stuff into terminal\n");
    printf("use backslash n to create a new line\n");

    // this is a comment, it is ignored by the compiler
    printf/*this is a comment*/("use backslash t to create a tab\t");
    printf("1\t2\t3\n4\t5\t6\n7\t8\t9\n");

    // VARIABLES

    // first we declare it, then we assign a value to it
    int a; //declaration    
    a = 5; //assignment
    int b = 10; //declaration and assignment

    int age = 16;                //integer
    float pi = 3.14;             //floating point number
    char letter = 'a';           //character
    char name[] = "John";        //array of characters

    printf("You are %d years old",age); // %d is a placeholder for an integer
    printf("Hello %s",name); // %s is a placeholder for a string
    printf("The value of pi is %f",pi); // %f is a placeholder for a float
    printf("The first letter of the alphabet is %c",letter); // %c is a placeholder for a character

    // DATA TYPES recap
    char a = 'a'; // single character %c
    char b[] = "hello"; // string %s
    int d = 5; // integer no decimal %d
    float c = 3.141592; // floating point number %f
    double e = 3.141592653589793; // double precision floating point number %lf
    bool f = true; // boolean %d (0 or 1)
    
    return 0;
}