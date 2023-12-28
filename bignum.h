#ifndef BIGNUM
#define BIGNUM

#include "vector.h"

/* 
    Macro: MIN(X, Y)
    ----------------
    Returns the minimum value between X and Y.
    If X is less than Y, it evaluates to X; otherwise, it evaluates to Y.
*/
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

/* 
    Macro: MAX(X, Y)
    ----------------
    Returns the maximum value between X and Y.
    If X is greater than Y, it evaluates to X; otherwise, it evaluates to Y.
*/
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

/* hexadecimal array with all 16 digits from '0' to 'F' */
extern const unsigned char hex_digits[];

#define PLUS 1      /* Positive sign bit */
#define MINUS -1    /* Negative sign bit */

/* BigNum implementation is based on the vector container */
typedef struct {
    int sign_bit;        /* Sign bit (PLUS or MINUS) */
    Vector_ptr digits;   /* Vector for storing the digits of the number in Little-Endian convention (lst on left)*/
    size_t num_system;   /* Number system (e.g., decimal, binary) */
} BigNum;

/* Defining BigNum_ptr as a pointer to a BigNum structure */
typedef BigNum *BigNum_ptr;

/* 
    Function: create_BigNum
    ------------------------
    Allocates memory for a new BigNum and initializes its properties.

    Returns:
    - A pointer to the newly created BigNum on success.
    - NULL_TYPE if memory allocation fails during BigNum creation.
*/
BigNum_ptr create_BigNum();

/* 
    Function: delete_BigNum
    ------------------------
    Deletes a BigNum and frees the associated memory.

    Parameters:
    - bg: A pointer to the BigNum to be deleted.

    Returns:
    - NULL_TYPE after freeing the BigNum's memory.
*/
BigNum_ptr delete_BigNum(BigNum_ptr bg);

/* 
    Function: assign_value
    ----------------------
    Assigns a new value to an existing BigNum based on a C string representation.

    Parameters:
    - bg: A pointer to the target BigNum.
    - num_cstring: C string representation of the number.
    - system: The number system in which the input is represented.

    Returns:
    - A pointer to the updated BigNum on success.
    - NULL_TYPE if there is an error, such as invalid input or memory allocation failure.
*/
BigNum_ptr assign_value(BigNum_ptr bg, const char *num_cstring, size_t system);

/* 
    Function: int_to_BigNum
    ------------------------
    Creates a new BigNum from an integer value.
    Number system set to decimal by default.

    Parameters:
    - val: The integer value.

    Returns:
    - A pointer to the newly created BigNum on success.
    - NULL_TYPE if there is an error, such as memory allocation failure.
*/
BigNum_ptr int_to_BigNum(int val);


/* 
    Function: copy_BigNum
    ----------------------
    Creates a new BigNum that is a copy of the provided BigNum.

    Parameters:
    - bg: A pointer to the source BigNum.

    Returns:
    - A pointer to the newly created BigNum (copy) on success.
    - NULL_TYPE if there is an error, such as memory allocation failure.
*/
BigNum_ptr copy_BigNum(BigNum_ptr bg);

/* 
    Function: clear_BigNum
    -----------------------
    Resets the value of a BigNum to NaN, usesy clear_vector.

    Parameters:
    - bg: A pointer to the target BigNum.
*/
void clear_BigNum(BigNum_ptr bg);

/* 
    Function: map_digit
    --------------------
    Maps an ASCII character representing a digit to its corresponding integer value.

    Parameters:
    - digit: The ASCII character representing a digit.

    Returns:
    - The integer value of the digit.
*/
int map_digit(unsigned char digit);

/* 
    Function: BigNum_sign
    ---------------------
    Gets the sign bit of a BigNum.

    Parameters:
    - bg: A pointer to the BigNum.

    Returns:
    - The sign bit (PLUS or MINUS).
*/
int BigNum_sign(BigNum_ptr bg);

/* 
    Function: BigNum_size
    ---------------------
    Gets the number of digits in a BigNum.

    Parameters:
    - bg: A pointer to the BigNum.

    Returns:
    - The number of digits in the BigNum.
*/
size_t BigNum_size(BigNum_ptr bg);

/* 
    Function: BigNum_base
    ---------------------
    Gets the number system (base) of a BigNum.

    Parameters:
    - bg: A pointer to the BigNum.

    Returns:
    - The number system (base) of the BigNum.
*/
size_t BigNum_base(BigNum_ptr bg);


/* 
    Function: zero_justify
    -----------------------
    Removes leading zero digits from a BigNum.

    Parameters:
    - bg: A pointer to the target BigNum.
*/
void zero_justify(BigNum_ptr bg);

//***************************************************************************************************************

/* 
    Function: add_BigNum
    ---------------------
    Adds two BigNums and returns the result.

    Parameters:
    - bg1: A pointer to the first BigNum.
    - bg2: A pointer to the second BigNum.

    Returns:
    - A pointer to the resulting BigNum on success.
    - NULL_TYPE if there is an error, such as invalid input or memory allocation failure.
*/
BigNum_ptr add_BigNum(BigNum_ptr bg1, BigNum_ptr bg2);

/* 
    Function: subtract_BigNum
    --------------------------
    Subtracts the second BigNum from the first and returns the result.

    Parameters:
    - bg1: A pointer to the first BigNum.
    - bg2: A pointer to the second BigNum.

    Returns:
    - A pointer to the resulting BigNum on success.
    - NULL_TYPE if there is an error, such as invalid input or memory allocation failure.
*/
BigNum_ptr subtract_BigNum(BigNum_ptr bg1, BigNum_ptr bg2);

/* 
    Function: multiply_BigNum
    --------------------------
    Multiplies two BigNums and returns the result.

    Parameters:
    - bg1: A pointer to the first BigNum.
    - bg2: A pointer to the second BigNum.

    Returns:
    - A pointer to the resulting BigNum on success.
    - NULL_TYPE if there is an error, such as invalid input or memory allocation failure.
*/
BigNum_ptr multiply_BigNum(BigNum_ptr bg1, BigNum_ptr bg2);

/* 
    Function: divide_BigNum
    ------------------------
    Divides the first BigNum by the second and returns the result.

    Parameters:
    - bg1: A pointer to the dividend BigNum.
    - bg2: A pointer to the divisor BigNum.

    Returns:
    - A pointer to the resulting BigNum on success.
    - NULL_TYPE if there is an error, such as division by zero or memory allocation failure.
*/
BigNum_ptr divide_BigNum(BigNum_ptr bg1, BigNum_ptr bg2);

/* 
    Function: exponentiate_BigNum
    ------------------------------
    Exponentiates the first BigNum by the second and returns the result.

    Parameters:
    - bg1: A pointer to the base BigNum.
    - bg2: A pointer to the exponent BigNum.

    Returns:
    - A pointer to the resulting BigNum on success.
    - NULL_TYPE if there is an error, such as invalid input or memory allocation failure.
*/
BigNum_ptr exponentiate_BigNum(BigNum_ptr bg1, BigNum_ptr bg2);

/* 
    Function: modulo_BigNum
    ------------------------
    Computes the modulo of the first BigNum by the second and returns the result.

    Parameters:
    - bg1: A pointer to the dividend BigNum.
    - bg2: A pointer to the divisor BigNum.

    Returns:
    - A pointer to the resulting BigNum on success.
    - NULL_TYPE if there is an error, such as division by zero or memory allocation failure.
*/
BigNum_ptr modulo_BigNum(BigNum_ptr bg1, BigNum_ptr bg2);

/* 
    Function: convert_system_BigNum
    --------------------------------
    Converts a BigNum to a different number system.

    Parameters:
    - bg: A pointer to the BigNum to be converted.
    - system: The target number system.

    Returns:
    - A pointer to the converted BigNum on success.
    - NULL_TYPE if there is an error, such as an unsupported number system or memory allocation failure.
*/
BigNum_ptr convert_system_BigNum(BigNum_ptr bg, size_t system);

//**************************************************************************************************************

/* 
    Function: compare_BigNum
    -------------------------
    Compares two BigNums

    Parameters:
    - bg1: A pointer to the first BigNum.
    - bg2: A pointer to the second BigNum.

    Returns:
    - -1 if bg1 < bg2.
    - 0 if bg1 == bg2.
    - 1 if bg1 > bg2.
*/
int compare_BigNum(BigNum_ptr bg1, BigNum_ptr bg2);

/* 
    Function: compare_abs_BigNum
    ----------------------------
    Compares the absolute values of two BigNums
    Parameters:
    - bg1: A pointer to the first BigNum.
    - bg2: A pointer to the second BigNum.

    Returns:
    - -1 if |bg1| < |bg2|.
    - 0 if |bg1| == |bg2|.
    - 1 if |bg1| > |bg2|.
*/
int compare_abs_BigNum(BigNum_ptr bg1, BigNum_ptr bg2);

/* 
    Function: BigNum_to_cstring
    ---------------------------
    Converts a BigNum to a C string representation.

    Parameters:
    - bg: A pointer to the BigNum to be converted.

    Returns:
    - A pointer to the C string representation on success.
    - NULL_TYPE if there is an error, such as memory allocation failure.
*/
char *BigNum_to_cstring(BigNum_ptr bg);

/* 
    Function: print_BigNum
    -----------------------
    Prints the array representation of a BigNum to the standard output.

    Parameters:
    - bg: A pointer to the BigNum to be printed.
*/
void print_BigNum(BigNum_ptr bg);

#endif