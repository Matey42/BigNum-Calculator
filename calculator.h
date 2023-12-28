#ifndef CALCULATOR
#define CALCULATOR

#include "vector.h"
#include "bignum.h"
#include <stdio.h>
#include <stdlib.h>


/*
    Enumeration: Operation
    -----------------------
    Defines possible operations for the calculator.
    - ADD: Addition '+'
    - SUBTRACT: Subtraction '-'
    - MULTIPLY: Multiplication '*'
    - DIVIDE: Division '/'
    - EXPONENTIATE: Exponentiation '^'
    - MODULO: Modulo '%'
    - CHANGE_BASE: Change number base '[2-16]' -> '[2-16]
    - UNDEFINED: Undefined operation
*/
enum Operation{
    ADD = 0,
    SUBSTRACT,
    MULTIPLY,
    DIVIDE,
    EXPONENTIATE,
    MODULO,
    CHANGE_BASE,
    UNDEFINED
};

/*
    Enumeration: Error_flag
    ------------------------
    Defines possible error flags for the calculator.
    - VALID: No error
    - INVALID_OPERATOR: Invalid operator
    - INVALID_BASE: Invalid base
    - INVALID_NUMBER_OF_ARG: Invalid number of arguments
    - INVALID_NUMBER: Invalid number
    - DIVISION_BY_ZERO: Division by zero
*/
enum Error_flag{
    VALID = 0,
    INVALID_OPERATOR,
    INVALID_BASE,
    INVALID_NUMBER_OF_ARG,
    INVALID_NUMBER,
    DIVISION_BY_ZERO
};

/* Represents a calculator with various properties for managing data */
typedef struct {
    BigNum_ptr memory;      // Memory for storing calculated results
    BigNum_ptr act_number;  // Active number for the current operation
    bool ready_result;      // Flag to indicate if the result is ready
    char *in_file;          // Input file name
    char *out_file;         // Output file name
    fpos_t index;           // File position index for reading input file
    enum Operation operation;   // Current operation
    enum Error_flag flag;       // Status flag for error handling
    size_t old_base;            // Base of the current number (for system conversion)
    size_t new_base;            // Target base for system conversion
} Calc;

/* Defining Calc_ptr as a pointer to a Calc structure*/
typedef Calc *Calc_ptr;

/*
    Function: create_calculator
    ---------------------------
    Allocates memory for a new calculator and initializes its properties.

    Parameters:
    - input_file: Name of the input file.
    - output_file: Name of the output file.

    Returns:
    - A pointer to the newly created calculator on success.
    - NULL_TYPE if memory allocation fails during calculator creation.
*/
Calc_ptr create_calculator(char* input_file, char* output_file);

/*
    Function: delete_calculator
    ---------------------------
    Deletes a calculator and frees the associated memory.

    Parameters:
    - ptr: A pointer to the calculator to be deleted.

    Returns:
    - NULL_TYPE after freeing the calculator's memory.
*/
Calc_ptr delete_calculator(Calc_ptr ptr);

/*
    Function: split_line
    --------------------
    Splits a line of text into an array of words.

    Parameters:
    - buffer: A character array containing the input line.

    Returns:
    - A dynamically allocated array of strings representing words in the line.
    - NULL if the line is empty.
*/
char **split_line(const char buffer[]);

/*
    Function: load_line
    -------------------
    Loads a line from the input file at a specific position.

    Parameters:
    - input_file: Name of the input file.
    - pos: Pointer to the file position index.

    Returns:
    - An array of strings representing the line on success.
    - NULL if file opening fails or if the line cannot be loaded.
*/
char **load_line(char *input_file, fpos_t *pos);

/*
    Function: line_memory_deallocation
    -----------------------------------
    Deallocates memory used by a line.

    Parameters:
    - ptr: A pointer to the array of strings representing a line.

    Returns:
    - NULL_TYPE after freeing the line's memory.
*/
char **line_memory_deallocation(char **ptr);

/*
    Function: words_in_line
    -----------------------
    Counts the number of words in a line.

    Parameters:
    - line: An array of strings representing a line.

    Returns:
    - The number of words in the line.
*/
int words_in_line(char **line);

/*
    Function: append_line
    ---------------------
    Appends a cstring to the output file.

    Parameters:
    - output_file: Name of the output file.
    - str: The string to be appended to the file.
*/
void append_line(char *output_file, char *str);

/*
    Function: save_line
    -------------------
    Concatenates element's of line and save them ussing append_line(..) to the output file.

    Parameters:
    - calc: A pointer to the calculator structure.
    - line: An array of strings representing a line.
*/
void save_line(Calc_ptr calc, char **line);

/*
    Function: save_error
    --------------------
    Saves an error message to the output file.

    Parameters:
    - calc: A pointer to the calculator structure.
*/
void save_error(Calc_ptr calc);


/*
    Function: is_base
    -----------------
    Checks if a string represents a valid base.

    Parameters:
    - str: The string to be checked.

    Returns:
    - true if the string represents a valid base, false otherwise.
*/
bool is_base(const char *str);

/*
    Function: is_num
    ----------------
    Checks if a string represents a valid number in a given base.

    Parameters:
    - str: The string to be checked.
    - num_base: The base for the number.

    Returns:
    - true if the string represents a valid number, false otherwise.
*/
bool is_num(const char *str, int num_base);

/*
    Function: update_operation_status
    ---------------------------------
    Updates the calc->operation status based on the input line.

    Parameters:
    - calc: A pointer to the calculator structure.
    - line: An array of strings representing a line.
*/
void update_operation_status(Calc_ptr calc, char** line);

/*
    Function: make_calculations
    ---------------------------
    Performs calculations based on the current operation.

    Parameters:
    - calc: A pointer to the calculator structure.
    - argument_counter: A pointer to the counter for the number of arguments.
*/
void make_calculations(Calc_ptr, int *argument_counter);

/*
    Function: start_calculations
    ----------------------------
    Starts the calculator and performs calculations from the input file.
    Used as a main program loop.

    Parameters:
    - ptr: A pointer to the calculator structure.
*/
void start_calculations(Calc_ptr ptr);


#endif