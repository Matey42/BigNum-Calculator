// Projekt zaliczeniowy z przedmiotu "Programowanie 1 - strukturalne" Informatyka PW MiNI
// Polecenie do kompilacji: gcc -std=c11 -Wall -Wextra -Werror -xc -lm vector.c bignum.c test.c -o test
// Polecenie uruchamiające program: ./test
// Autor: Mateusz Kozłowski
// Data: XX.11.2023

// W celu przetestowania odpowiednich funkcji oraz struktur - odkomentować poniższy kod "#define RUN_TESTS"

#define RUN_TESTS

#ifdef RUN_TESTS

#define VECTOR_TEST // <== Odkomentować w celu przetesowania funkcji składowych Vectora
#define BIGNUM_TEST // <== Odkomentować w celu przetesowania funkcji składowych BigNum'a

#include "vector.h"
#include "bignum.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void vector_test()
{
    printf("\n======= Testing Vector Capabilities =======\n\n");

    Vector_ptr vec = create_vector();
    printf("Creating vector --> ");
    print_vector(vec);

    printf("* Filling vector --> ");
    for(size_t i = 1; i <= 10; i++)
        push_back(vec, i*i);
    
    print_vector(vec);
    printf("Parametrs of vector size=%u, capacity=%u\n", vector_size(vec), vector_capacity(vec));
    
    printf("* Reversed vector --> ");
    reverse_vector(vec);
    print_vector(vec);

    pop_back(vec);
    vector_set(vec, 5, 13);
    printf("* Setting vec[5]= %u and removing last element -->", vector_get(vec, 5));
    print_vector(vec);

    printf("* Clearing vector --> ");
    vector_clear(vec);
    print_vector(vec);
    printf("Is vector empty? %s\n", (vector_empty(vec) ? "True" : "False") );
    printf(" ---> Parametrs of vector size=%u, capacity=%u\n", vector_size(vec), vector_capacity(vec));
    push_back(vec, 37);
    push_back(vec, 75);

    printf("Pushing 2 elements into vector: front= %u, back= %u, size= %u, capacity= %u\n",
            vector_front(vec), vector_back(vec), vector_size(vec), vector_capacity(vec));

    print_vector(vec);

    printf("Is vector empty? %s \n", (vector_empty(vec) ? "True" : "False") );


    vec = delete_vector(vec);

    if(vec == NULL)
        printf("Vector deleted\n");

    printf("\n===========================================\n\n");
}

void bignum_test() {
    printf("\n======= Testing BigNum Capabilities =======\n\n");

    // Create a BigNum
    BigNum_ptr bg1 = create_BigNum();
    assign_value(bg1, "123", 9);
    printf("Assigning value: bg1 = (123)[9]: ");
    print_BigNum(bg1);

    // Create another BigNum
    BigNum_ptr bg2 = int_to_BigNum(-125); // Decimal system
    convert_system_BigNum(bg2, 9);
    printf("Assigning value: bg2 = -(125)[9]: ");
    print_BigNum(bg2);

        // Add two BigNums
    add_BigNum(bg1, bg2);
    printf("bg1 + bg2 = ");
    print_BigNum(bg1);

    // Subtract two BigNums
    subtract_BigNum(bg2, bg1);
    printf("bg2 - bg1 = ");
    print_BigNum(bg2);
    
    // Multiply two BigNums
    multiply_BigNum(bg1, bg2);
    printf("bg1 * bg2 = ");
    print_BigNum(bg1);

    // Assign new values to BigNums
    printf("\nAssigning new values:\n");
    assign_value(bg1, "26", 10);
    assign_value(bg2, "3", 10);

    printf("bg1: ");
    print_BigNum(bg1);

    printf("bg2: ");
    print_BigNum(bg2);

    // Divide two BigNums
    divide_BigNum(bg1, bg2);
    printf("bg1 / bg2 = ");
    print_BigNum(bg1);

    // Modulo of two BigNums
    modulo_BigNum(bg1, bg2);
    printf("bg1 % bg2 = ");
    print_BigNum(bg1);

    // Assign a new value in hexadecimal
    assign_value(bg1, "FB", 16);
    printf("\n\nAssigned FB to BigNum (hexadecimal): ");
    print_BigNum(bg1);

    // Compare BigNums
    assign_value(bg2, "-FB", 16);
    printf("Created BigNum2 (hexadecimal): ");
    print_BigNum(bg2);
    int comparison = compare_BigNum(bg1, bg2);
    printf("Comparison result (BigNum1 cmp BigNum2): %d\n", comparison);
    printf("|ABS| comparison result ( |BigNum1| cmp |BigNum2|): %d\n", compare_abs_BigNum(bg1, bg2));

    printf("\n++++++++++++++++++++++++++++++++++++++++++\n");

    // Convert BigNum to a C string
    char *str = BigNum_to_cstring(bg2);
    printf("BigNum2: %s\n", str);
    free(str);

    // Convert BigNum to a different number system
    convert_system_BigNum(bg2, 10);
    printf("Converting BigNum2 (to decimal): ");
    print_BigNum(bg2);

    // Convert BigNum to a different base
    assign_value(bg1, "540263", 7);
    printf("\nConverting BigNum1:");
    print_BigNum(bg1);
    printf(" ==> ");
    convert_system_BigNum(bg1, 12);
    print_BigNum(bg1);

    // Exponentiate BigNum
    assign_value(bg1, "3", 10);
    assign_value(bg2, "10", 10);
    printf("bg1 => ");
    print_BigNum(bg1);
    printf("bg2 => ");
    print_BigNum(bg2);
    exponentiate_BigNum(bg1, bg2);
    printf("bg1 ^ bg2 [3 ^ 10] = ");
    print_BigNum(bg1);

    // Cleanup
    delete_BigNum(bg1);
    delete_BigNum(bg2);

    printf("\n===========================================\n\n");
}


int main(){

#ifdef VECTOR_TEST
    vector_test();
#endif

#ifdef BIGNUM_TEST
    bignum_test();
#endif

    return 0;
}

#endif
