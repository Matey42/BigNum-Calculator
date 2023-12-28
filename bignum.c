#include "bignum.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define DECIMAL_BASE 10

typedef unsigned char uc;


/* hexadecimal array with all 16 digits from '0' to 'F' */
const unsigned char hex_digits[] = 
{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/* Allocates memory for a new BigNum and initializes its properties.
Returns:
- A pointer to the newly created BigNum on success.
- NULL_TYPE if memory allocation fails during BigNum creation*/
BigNum_ptr create_BigNum()
{
    BigNum_ptr bg = (BigNum_ptr)malloc(sizeof(BigNum));
    if(bg == NULL || (bg->digits = create_vector()) == NULL_TYPE){
        fprintf(stderr, "Memory allocation failed during BigNum creation\n");
        return NULL_TYPE;
    }

    bg->sign_bit = PLUS;
    bg->num_system = 2;
    return bg;
}

/* Delete a BigNum and free associated memory.
 Returns: NULL_TYPE after freeing the BigNum's memory.*/
BigNum_ptr delete_BigNum(BigNum_ptr bg)
{
    if(bg != NULL){
        delete_vector(bg->digits);
        free(bg);
    }

    return NULL_TYPE;
}

/* Assigns a new value to an existing BigNum based on a C string representation.
Returns:
- A pointer to the updated BigNum on success.
- NULL_TYPE if there is an error, such as invalid input or memory allocation failure.*/
BigNum_ptr assign_value(BigNum_ptr bg, const char *num_cstring, size_t system)
{
    if(bg == NULL || num_cstring == NULL || num_cstring[0] == '\0'){
        fprintf(stderr, "Invalid BigNum or C string\n");
        return NULL_TYPE;
    }

    clear_BigNum(bg);

    bg->num_system = system;
    switch(num_cstring[0])
    {
        case '-':
            bg->sign_bit = -1;
            num_cstring++;
            break;
        case '+':
            bg->sign_bit = 1;
            num_cstring++;
            break;
        default:
            bg->sign_bit = 1;
    }

    uc num;
    while( (num = *num_cstring++) != '\0')
    {
        uc val = map_digit(num);

        if(val >= system){
            fprintf(stderr, "The number is invalid in specified num system\n");
            return NULL_TYPE;
        }

        push_back(bg->digits, val);
    }

    reverse_vector(bg->digits);
    zero_justify(bg);

    return bg;
}

/* Creates a new BigNum from an integer value. 
Returns: A pointer to the newly created BigNum on success, NULL_TYPE otherwise*/
BigNum_ptr int_to_BigNum(int val)
{
    int max_representation_lenght = 34;
    char str_val[max_representation_lenght];
    sprintf(str_val, "%d", val);

    BigNum_ptr ptr = create_BigNum();
    return assign_value(ptr, str_val, DECIMAL_BASE);
}

/* Creates a new BigNum that is a copy of the provided BigNum.
Returns: A pointer to the newly created BigNum (via copy) on success, NULL_TYPE otherwise*/
BigNum_ptr copy_BigNum(BigNum_ptr bg)
{
    if(bg == NULL){
        fprintf(stderr, "Invalid BigNum_ptr\n");
        return 0;
    }

    BigNum_ptr copy; 
    if((copy = create_BigNum()) == NULL_TYPE)
        return NULL_TYPE;
    
    if((copy->digits = copy_vector(bg->digits)) == NULL_TYPE){
        free(copy);
        return NULL_TYPE;
    }
    
    copy->num_system = bg->num_system;
    copy->sign_bit = bg->sign_bit;

    return copy;
}

/*Resets the value of a BigNum to NaN, usesy clear_vector.*/
void clear_BigNum(BigNum_ptr bg)
{
    if(bg == NULL){
        fprintf(stderr, "Invalid BigNum_ptr\n");
        return;
    }

    bg->sign_bit = PLUS;
    vector_clear(bg->digits);
}

/* Maps an ASCII character representing a digit to its corresponding integer value. */
int map_digit(unsigned char digit)
{
    uc val;
    if(isdigit(digit))
    {
        val = digit - '0';
    }
    else
    {
        switch(digit)
        {
            case 'A':
                val = 10;
                break;
            case 'B':
                val = 11;
                break;
            case 'C':
                val = 12;
                break;
            case 'D':
                val = 13;
                break;
            case 'E':
                val = 14;
                break;
            case 'F':
                val = 15;
                break;
            default:
                val = 255;
        }
    }

    return val;
}

/* Gets the sign bit of a BigNum. */
int BigNum_sign(BigNum_ptr bg)
{
    if(bg == NULL){
        fprintf(stderr, "Invalid BigNum_ptr\n");
        return 0;
    }

    return bg->sign_bit;
}

/* Gets the number of digits in a BigNum. */
size_t BigNum_size(BigNum_ptr bg)
{
    if(bg == NULL){
        fprintf(stderr, "Invalid BigNum_ptr\n");
        return 0;
    }

    return vector_size(bg->digits);
}

/* Gets the number system (base) of a BigNum. */
size_t BigNum_base(BigNum_ptr bg)
{
    if(bg == NULL){
        fprintf(stderr, "Invalid BigNum_ptr\n");
        return 0;
    }

    return bg->num_system;
}

/* Removes leading zero digits from a BigNum. */
void zero_justify(BigNum_ptr bg)
{
    if(bg == NULL){
        fprintf(stderr, "Invalid BigNum_ptr\n");
        return;
    }

    while(BigNum_size(bg) > 1 && vector_back(bg->digits) == 0){
        pop_back(bg->digits);
    }

    if(BigNum_size(bg) == 1 && vector_back(bg->digits) == 0){
        bg->sign_bit = PLUS;
    }
}

/* Add two BigNums and return the result. 
 Uses school method adding under the line */
BigNum_ptr add_BigNum(BigNum_ptr bg1, BigNum_ptr bg2)
{
    // Check for valid input and compatible bases
    if (bg1 == NULL || bg2 == NULL || BigNum_base(bg1) != BigNum_base(bg2)) {
        fprintf(stderr, "Invalid BigNum_ptrs or numbers coded in different bases\n");
        return NULL_TYPE;
    }

    BigNum_ptr bg;

    // Handle cases where signs are different
    if (BigNum_sign(bg1) != BigNum_sign(bg2)) {
        if (BigNum_sign(bg1) == MINUS) {
            bg1->sign_bit = PLUS;
            bg = subtract_BigNum(bg1, bg2);
            bg1->sign_bit = MINUS;
        } else {
            bg2->sign_bit = PLUS;
            bg = subtract_BigNum(bg1, bg2);
            bg2->sign_bit = MINUS;
        }

        return bg;
    }

    // Ensure both vectors have the same size for addition
    size_t max_size = MAX(BigNum_size(bg1), BigNum_size(bg2)) + 1;
    vector_resize(bg1->digits, max_size);
    vector_resize(bg2->digits, max_size);

    int carry = 0;

    // Perform addition
    for (size_t i = 0; i < max_size; ++i) {
        int act_val = carry + vector_get(bg1->digits, i) + vector_get(bg2->digits, i);
        vector_set(bg1->digits, i, (DATA_TYPE)(act_val % BigNum_base(bg2)));
        carry = act_val / BigNum_base(bg2);
    }

    // Zero-justify the vectors
    zero_justify(bg1);
    zero_justify(bg2);

    return bg1;
}

/* Subtracts the second BigNum from the first and returns the result. 
    Uses school method subtracting under the line */
BigNum_ptr subtract_BigNum(BigNum_ptr bg1, BigNum_ptr bg2)
{
    // Check for valid input and compatible bases
    if (bg1 == NULL || bg2 == NULL || BigNum_base(bg1) != BigNum_base(bg2)) {
        fprintf(stderr, "Invalid BigNum_ptrs or numbers coded in different bases\n");
        return NULL_TYPE;
    }

    // Handle cases where one of the numbers is negative
    if (BigNum_sign(bg1) == MINUS || BigNum_sign(bg2) == MINUS) {
        bg2->sign_bit *= -1;
        add_BigNum(bg1, bg2);
        bg2->sign_bit *= -1;
        return bg1;
    }

    Vector_ptr vec;
    Vector_ptr subtrahend;

    // Determine which number is greater in absolute value
    switch (compare_abs_BigNum(bg1, bg2)) {
        case 1:
            vec = copy_vector(bg1->digits);
            subtrahend = bg2->digits;
            break;
        case -1:
            vec = copy_vector(bg2->digits);
            subtrahend = bg1->digits;
            bg1->sign_bit = MINUS;
            break;
        case 0:
            // Numbers are equal, result is 0
            clear_BigNum(bg1);
            push_back(bg1->digits, 0);
            return bg1;
        default:
            fprintf(stderr, "Numbers coded in different bases\n");
            return NULL_TYPE;
    }

    if (vec == NULL_TYPE)
        return NULL_TYPE;

    // Perform subtraction [ vec - subtrahend ]
    int borrow = 0;
    size_t n = MAX(vector_size(vec), vector_size(subtrahend));

    vector_resize(vec, n);
    vector_resize(subtrahend, n);

    for (size_t i = 0; i < n; ++i) {
        int act_val = vector_get(vec, i) - borrow - vector_get(subtrahend, i);

        if (vector_get(vec, i) > 0)
            borrow = 0;

        if (act_val < 0) {
            act_val += BigNum_base(bg2);
            borrow = 1;
        }

        vector_set(vec, i, (DATA_TYPE)act_val % BigNum_base(bg2));
    }

    // Replace the digits in bg1 with the result
    delete_vector(bg1->digits);
    bg1->digits = vec;

    // Zero-justify the vectors
    zero_justify(bg1);
    zero_justify(bg2);

    return bg1;
}


/* Shifts the elements in a vector to the right by a specified number of positions.*/
void digit_shift(Vector_ptr ptr, int n)
{
    // Check for invalid vector or negative shift (right shift only)
    if (ptr == NULL || n < 0) {
        fprintf(stderr, "Invalid vector or negative shift (possible only for right shift)\n");
        return;
    }

    // If the vector has a single element and it is 0, no shift is needed
    if (vector_size(ptr) == 1 && vector_front(ptr) == 0)
        return;

    // Resize the vector to accommodate the shift
    vector_resize(ptr, vector_size(ptr) + n);

    // Perform the shift
    for (int i = vector_size(ptr) - n - 1; i >= 0; --i)
        ptr->array[i + n] = ptr->array[i];

    // Fill the vacated positions with 0
    for (int i = 0; i < n; ++i)
        ptr->array[i] = 0;
}

/* Multiply two BigNums and return the result. 
 Uses school method multiplying under the line */
BigNum_ptr multiply_BigNum(BigNum_ptr bg1, BigNum_ptr bg2)
{
    // Check for invalid BigNum_ptrs or numbers coded in different bases
    if (bg1 == NULL || bg2 == NULL || BigNum_base(bg1) != BigNum_base(bg2)) {
        fprintf(stderr, "Invalid BigNum_ptrs or numbers coded in different bases\n");
        return NULL_TYPE;
    }

    // Create a copy of bg1 to use for the multiplication process
    BigNum_ptr row = copy_BigNum(bg1);
    
    // Clear the original value of bg1 and set its sign bit
    clear_BigNum(bg1);
    bg1->sign_bit = row->sign_bit;

    // Perform multiplication using the school method
    for (size_t i = 0; i < BigNum_size(bg2); ++i) {
        DATA_TYPE digit = vector_get(bg2->digits, i);
        for (size_t j = 1; j <= digit; ++j) {
            add_BigNum(bg1, row);
        }
        digit_shift(row->digits, 1);
    }

    // Adjust the sign bit of the result
    bg1->sign_bit *= bg2->sign_bit;

    // Remove leading zeros
    zero_justify(bg1);

    return bg1;
}

/* Divide two BigNums and return the result. */
BigNum_ptr divide_BigNum(BigNum_ptr bg1, BigNum_ptr bg2)
{
    // Check for invalid BigNum_ptrs or numbers coded in different bases
    if (bg1 == NULL || bg2 == NULL || BigNum_base(bg1) != BigNum_base(bg2)) {
        fprintf(stderr, "Invalid BigNum_ptrs or numbers coded in different bases\n");
        return NULL_TYPE;
    }

    // Check for division by zero
    if (BigNum_size(bg2) == 1 && vector_get(bg2->digits, 0) == 0) {
        fprintf(stderr, "Division by 0 - operation invalid\n");
        return NULL_TYPE;
    }

    // Save the sign bits of bg1 and bg2
    int bg1_sign = bg1->sign_bit;
    int bg2_sign = bg2->sign_bit;

    // Set sign bits to positive for the division process
    bg1->sign_bit = PLUS;
    bg2->sign_bit = PLUS;

    // Create a BigNum to represent the current portion of bg1 during division
    BigNum_ptr row = create_BigNum();
    row->num_system = bg2->num_system;

    for (int i = BigNum_size(bg1) - 1; i >= 0; --i) {
        // Shift the current portion of bg1 to the left ("cropping the lsb")
        digit_shift(row->digits, 1);
        row->digits->array[0] = vector_get(bg1->digits, i);
        vector_set(bg1->digits, i, 0);

        // Perform subtraction until the current portion is less than bg2
        while (compare_abs_BigNum(row, bg2) != -1) {
            bg1->digits->array[i]++;
            subtract_BigNum(row, bg2);
        }
    }

    // Remove leading zeros and adjust the sign bit of the result
    zero_justify(bg1);
    bg1->sign_bit = bg1_sign * bg2_sign;
    bg2->sign_bit = bg2_sign;

    return bg1;
}

/* Exponentiates the first BigNum by the second and returns the result. */
BigNum_ptr exponentiate_BigNum(BigNum_ptr bg1, BigNum_ptr bg2)
{
    // Check for invalid BigNum_ptrs or numbers coded in different bases
    if (bg1 == NULL || bg2 == NULL || BigNum_base(bg1) != BigNum_base(bg2)) {
        fprintf(stderr, "Invalid BigNum_ptrs or numbers coded in different bases\n");
        return NULL_TYPE;
    }

    // Check for a negative exponent
    if (BigNum_sign(bg2) == MINUS) {
        fprintf(stderr, "Invalid exponent - Negative value\n");
        return NULL_TYPE;
    }

    /*
    Algorithm:
    Write the exponent n in binary. Read the binary representation from left to right, starting with the second bit from the left.
    Start with the number a, and every time you read a 0 bit, square what you’ve got. Every time you read a 1 bit,
    square what you’ve got and multiply by a. It follows that an can be computed using no more than 2 log2(n) multiplications.
    */

    BigNum_ptr exp = copy_BigNum(bg2);
    convert_system_BigNum(exp, 2);
    // pop_back(exp->digits);

    BigNum_ptr result = copy_BigNum(bg1);
    // print_BigNum(result);
    // printf("EXP - ");
    // print_BigNum(exp);
    for(pop_back(exp->digits); BigNum_size(exp) > 0; pop_back(exp->digits))
    {
        // printf("RESULT: ");
        // print_BigNum(result);

        BigNum_ptr tmp = copy_BigNum(result);
        switch(vector_back(exp->digits))
        {
            case 0:
                multiply_BigNum(result, tmp);
                break;
            case 1:
                multiply_BigNum(result, tmp);
                multiply_BigNum(result, bg1);
                break;
            default:
                fprintf(stderr, "Invalid exponent in exponentiate_BigNum\n");
                return NULL_TYPE;
        }
        delete_BigNum(tmp);
    }

    // printf("RESULT: ");

    vector_clear(bg1->digits);
    bg1->digits = result->digits;
    result->digits = NULL_TYPE;

    delete_BigNum(exp);
    delete_BigNum(result);

    return bg1;
}

/* Computes the modulo of the first BigNum by the second and returns the result. 
  Using methamatic formula: r = a - d * floor(a/d)*/
BigNum_ptr modulo_BigNum(BigNum_ptr bg1, BigNum_ptr bg2)
{
    if(bg1 == NULL || bg2 == NULL || BigNum_base(bg1) != BigNum_base(bg2)){
        fprintf(stderr, "Invalid BigNum_ptr's or numbers coded in diffrent bases\n");
        return NULL_TYPE;
    }

    if(BigNum_size(bg2) == 1 && vector_get(bg2->digits, 0) == 0){
        fprintf(stderr, "Division by 0 - operation invalid\n");
        return NULL_TYPE;
    }

    if(BigNum_sign(bg1) == MINUS || BigNum_sign(bg2) == MINUS){
        fprintf(stderr, "Modulo for negative numbers not defined\n");
        return NULL_TYPE;
    }

    // r = a - d * floor(a/d)

    // floor(a/d)
    BigNum_ptr tmp = copy_BigNum(bg1);

    divide_BigNum(tmp, bg2);
    
    // d * floor(a/d)
    multiply_BigNum(tmp, bg2);
    
    // a - d * floor(a/d)
    subtract_BigNum(bg1, tmp);

    delete_BigNum(tmp);

    return bg1;
}

/* Convert a BigNum to a different number system. 
  The function first converts the BigNum to the decimal system, then converts it to the target system.
  Supports number systems in the range [2, 16]. */
BigNum_ptr convert_system_BigNum(BigNum_ptr bg, size_t base_system)
{
    // Check for invalid BigNum or number system
    if (bg == NULL || base_system < 2 || base_system > 16) {
        fprintf(stderr, "Invalid BigNum or number system\n");
        return NULL_TYPE;
    }

    // No conversion needed if the systems are the same
    if (bg->num_system == base_system)
        return bg;

    // Initialize constants for decimal system and create a result BigNum
    
    BigNum_ptr result = create_BigNum();
    assign_value(result, "0", DECIMAL_BASE);

    // Convert the BigNum to the decimal system
    BigNum_ptr multiplicant = int_to_BigNum(1);
    BigNum_ptr num_system = int_to_BigNum(bg->num_system);
    bg->num_system = DECIMAL_BASE;

    BigNum_ptr tmp;

    for (size_t i = 0; i < BigNum_size(bg); ++i) {
        tmp = int_to_BigNum(vector_get(bg->digits, i));
        multiply_BigNum(tmp, multiplicant);
        add_BigNum(result, tmp);
        multiply_BigNum(multiplicant, num_system);
        delete_BigNum(tmp);
    }

    // Clear the original digits and update the number system
    vector_clear(bg->digits);
    delete_BigNum(multiplicant);
    delete_BigNum(num_system);

    // If the target system is decimal, update the original BigNum and return
    if (base_system == DECIMAL_BASE) {
        bg->digits = result->digits;
        result->digits = NULL_TYPE;
        delete_BigNum(result);
        zero_justify(bg);
        return bg;
    }

    // Set the target system to decimal and initialize constants for the target system
    num_system = int_to_BigNum(base_system);
    BigNum_ptr zero = create_BigNum();
    assign_value(zero, "0", DECIMAL_BASE);

    // Convert the BigNum to the target system
    while (compare_abs_BigNum(result, zero) == PLUS) {
        tmp = copy_BigNum(result);
        modulo_BigNum(tmp, num_system);
        
        char *num = BigNum_to_cstring(tmp);
        push_back(bg->digits, atoi(num));
        free(num);

        divide_BigNum(result, num_system);
        delete_BigNum(tmp);
    }

    // Clean up
    delete_BigNum(tmp);
    delete_BigNum(num_system);
    delete_BigNum(zero);

    // Update the number system and perform zero justification
    bg->num_system = base_system;
    zero_justify(bg);

    return bg;
}


/* Compare two BigNums and return -1 if bg1 < bg2, 0 if bg1 == bg2, or 1 if bg1 > bg2. */
int compare_BigNum(BigNum_ptr bg1, BigNum_ptr bg2)
{
    // Check for invalid BigNum pointers or numbers coded in different bases
    if (bg1 == NULL || bg2 == NULL || BigNum_base(bg1) != BigNum_base(bg2)) {
        fprintf(stderr, "Invalid BigNum_ptr's or numbers coded in different bases\n");
        return -2;
    }

    // Handle different signs
    if (bg1->sign_bit == PLUS && bg2->sign_bit == MINUS)
        return 1;

    if (bg1->sign_bit == MINUS && bg2->sign_bit == PLUS)
        return -1;

    // Same signs
    return compare_abs_BigNum(bg1, bg2) * bg1->sign_bit;
}

/* Compare the absolute values of two BigNums and return -1 if |bg1| < |bg2|, 0 if |bg1| == |bg2|, or 1 if |bg1| > |bg2|. */
int compare_abs_BigNum(BigNum_ptr bg1, BigNum_ptr bg2)
{
    // Check for invalid BigNum pointers or numbers coded in different bases
    if (bg1 == NULL || bg2 == NULL || BigNum_base(bg1) != BigNum_base(bg2)) {
        fprintf(stderr, "Invalid BigNum_ptr's or numbers coded in different bases\n");
        return -2;
    }

    // Compare sizes
    if (BigNum_size(bg1) > BigNum_size(bg2)) {
        return 1;
    } else if (BigNum_size(bg1) == BigNum_size(bg2)) {
        size_t n = BigNum_size(bg1);

        // Compare digits from most significant to least significant
        for (size_t i = 0; i < n; ++i) {
            if (vector_get(bg1->digits, n - i - 1) > vector_get(bg2->digits, n - i - 1))
                return 1;
            else if (vector_get(bg1->digits, n - i - 1) < vector_get(bg2->digits, n - i - 1))
                return -1;
        }

        return 0;
    }

    return -1;
}


/* Convert a BigNum to a C string representation. */
char *BigNum_to_cstring(BigNum_ptr bg)
{
    // Check for invalid BigNum pointer
    if (bg == NULL) {
        fprintf(stderr, "Invalid BigNum\n");
        return NULL_TYPE;
    }

    size_t n = BigNum_size(bg);
    // Allocate memory for the C string representation
    // First index reserved for sign, last for null character
    char *str = (char *)calloc(n + 2, sizeof(char));

    // Check for memory allocation failure
    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed during vector converting BigNum to C-string\n");
        return NULL_TYPE;
    }

    // Convert BigNum to C string
    if (BigNum_sign(bg) == PLUS) {
        for (size_t i = 0; i < n; ++i)
            str[i] = hex_digits[vector_get(bg->digits, n - i - 1)];
        str[n] = '\0';
    } else {
        for (size_t i = 1; i <= n; ++i)
            str[i] = hex_digits[vector_get(bg->digits, n - i)];
        str[0] = '-';
        str[n + 1] = '\0';
    }

    return str;
}

/*Prints the BigNum in array representation (as stored in memory) to a standard stream.*/ 
void print_BigNum(BigNum_ptr bg)
{
    // Check for invalid BigNum pointer
    if (bg == NULL) {
        fprintf(stderr, "Invalid BigNum\n");
        return;
    }

    // Print BigNum information
    printf("BigNum: (BASE %d) %c ", BigNum_base(bg), (BigNum_sign(bg) == PLUS ? '+' : '-'));

    // Print the array representation of the BigNum
    print_vector(bg->digits);
}