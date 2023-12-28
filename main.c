// Projekt zaliczeniowy z przedmiotu "Programowanie 1 - strukturalne" Informatyka PW MiNI
// Polecenie do kompilacji:  gcc -std=c11 -Wall -Wextra -Werror -xc -lm vector.c bignum.c calculator.c main.c -o calc
// Polecenie uruchamiające program: ./calc.exe
// Autor: Mateusz Kozłowski
// Data: XX.11.2023


#include "vector.h"
#include "bignum.h"
#include "calculator.h"
#include <string.h>


/* 
    Function: generate_output_name
    ------------------------------
    Generates the output file name based on the input file name.

    Parameters:
    - input_file: A pointer to the input file name.

    Returns:
    - A pointer to the generated output file name on success.
    - NULL if there is a memory allocation failure.
*/
char *generate_output_name(const char *input_file) {
    // Calculate the length of the input string
    size_t input_length = strlen(input_file);

    // Allocate memory for the output string
    char *str = (char *)malloc(input_length + 5);  // +5 for "out_" and the null terminator

    // Check for memory allocation failure
    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Copy "out_" to the beginning of the output string
    strncpy(str, "out_", 4);

    // Copy the input string to the output string
    strncpy(str + 4, input_file, input_length);

    // Null-terminate the output string
    str[input_length + 4] = '\0';

    return str;
}

int main(int argc, char **argv) {
    char *input_file;
    char *output_file;

    // Check the number of arguments
    if (argc == 2) {
        // argv[0] reserved for program name
        input_file = argv[1]; // first argument
        output_file = generate_output_name(input_file);
    } else if (argc == 3) {
        input_file = argv[1]; // first argument
        output_file = argv[2]; // second argument
    } else {
        fprintf(stderr, "Number of entered arguments is incorrect\n");
        return -1;
    }

    // Check if the input file can be opened
    FILE *fptr;
    if ((fptr = fopen(input_file, "r")) == NULL) {
        fprintf(stderr, "Problem with opening the input_file\n");
        return -1;
    }
    fclose(fptr);

    // Check if the output file can be opened
    if ((fptr = fopen(output_file, "w")) == NULL) {
        fprintf(stderr, "Problem with opening the output_file\n");
        return -1;
    }
    fclose(fptr);

    // Create a calculator and start calculations
    Calc_ptr calc;
    if ((calc = create_calculator(input_file, output_file)) == NULL) {
        return -1;
    }

    start_calculations(calc);

    // Cleanup
    delete_calculator(calc);

    return 0;
}