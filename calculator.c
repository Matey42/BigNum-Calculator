#include "calculator.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

// uncomment for debugging purposes
// #define DEBUG

/*  Allocates memory for a new calculator and initializes its properties.
Returns: A pointer to the newly created calculator on success, NULL_TYPE otherwise. */
Calc_ptr create_calculator(char* input_file, char* output_file)
{
    Calc_ptr calc = (Calc_ptr)malloc(sizeof(Calc));
    if(calc == NULL){
        fprintf(stderr, "Memory allocation failed during Calculator creation\n");
        return NULL_TYPE;
    }

    calc->memory = create_BigNum();
    calc->act_number = create_BigNum();
    calc->ready_result = false;

    calc->in_file = input_file;
    calc->out_file = output_file;
    
    calc->operation = UNDEFINED;
    calc->flag = VALID;

    calc->index = 0;
    
    FILE *file;
    file = fopen(input_file, "r");
    if(file == NULL){
        fprintf(stderr, "File oppening failed\n");
        return NULL_TYPE;
    }

    fgetpos(file, &calc->index);

    fclose(file);

    return calc;
}

/*  Deletes a calculator and frees the associated memory.
    Returns: NULL_TYPE after freeing the calculator's memory. */
Calc_ptr delete_calculator(Calc_ptr calc)
{
    if(calc != NULL){
        delete_BigNum(calc->memory);
        delete_BigNum(calc->act_number);

        free(calc);
    }

    return NULL_TYPE;
}

/*Counts the number of words in a given buffer.*/
int count_words(const char buffer[])
{
    int counter = 0;

    for(int i = 0; buffer[i] != '\n'; ++i)
    {
        // Check if the current character is not a whitespace, and the next character is a whitespace
        if(!isspace(buffer[i]) && isspace(buffer[i+1]))
            ++counter;
    }
    return counter;
}

/* Splits a line of text into an array of words. 
Returns:
- A dynamically allocated array of strings representing words in the line (NULL terminated).
- NULL if the line is empty.*/
char **split_line(const char buffer[])
{
    int n = count_words(buffer); // Count the number of words in the line
    if (n == 0) return NULL; // If no words found, return NULL

    char **ptr = (char**)calloc(n + 1, sizeof(char*)); // Allocate memory for an array of strings

    size_t l_ptr = 0, r_ptr = 0; // Initialize left and right pointers for word extraction

    for (int i = 0; i < n; ++i)
    {
        // Skip leading spaces
        while (isspace(buffer[r_ptr])) ++l_ptr, ++r_ptr;

        // Find the end of the current word
        while (!isspace(buffer[r_ptr])) ++r_ptr;

        int dist = r_ptr - l_ptr + 1; // Calculate the length of the current word
        ptr[i] = (char*)calloc(dist, sizeof(char)); // Allocate memory for the word

        for (int x = 0; l_ptr < r_ptr; ++x)
        {
            ptr[i][x] = buffer[l_ptr]; // Copy characters from buffer to the word
            ++l_ptr;
        }
    }

    return ptr; // Return the array of words
}

/* Loads a line from a file, splits it into words, and returns an array of words. */
char **load_line(char* input_file, fpos_t *pos)
{
    FILE *file;
    file = fopen(input_file, "r"); // Open the file in read mode
    if(file == NULL){
        fprintf(stderr, "File opening failed\n");
        return NULL; // Return NULL if file opening fails
    }

    if (fsetpos(file, pos) != 0) {
        fprintf(stderr, "Error setting file position");
        return NULL; // Return NULL if there is an error in setting the file position
    }

    char buffer[1024] = {'\0'}; // Initialize a buffer to store the line
    char **ptr = NULL; // Initialize a pointer to an array of words

    while (ptr == NULL) {
        // Read a line from the file and split it into words
        if (fgets(buffer, sizeof(buffer), file) != NULL)
            ptr = split_line(buffer);
        else
            return NULL; // Return NULL if there is an error or end of file is reached
    }

    if (fgetpos(file, pos) != 0) {
        fprintf(stderr, "Error getting file position");
        return NULL; // Return NULL if there is an error in getting the file position
    }

    fclose(file); // Close the file

    return ptr; // Return the array of words
}

/*  Deallocates memory occupied by an array of strings. Return NULL*/
char** line_memory_deallocation(char **ptr)
{   
    if(ptr != NULL){
        for(int i = 0; *(ptr + i) != NULL; ++i)   
            free(*(ptr + i)); // Free memory for each string in the array
        free(ptr); // Free memory for the array of strings
    }

    return NULL_TYPE; // Return NULL_TYPE after freeing the memory
}

/* Counts the number of words in an array of strings. */
int words_in_line(char **line)
{
    int word_counter = 0;
    for(int i = 0; line[i] != NULL; ++i)
        ++word_counter;
    
    return word_counter;
}

/* Appends a string to the end of a file. */
void append_line(char *output_file, char *str) {
    FILE *file;
    
    // Attempt to open the file in append mode
    if ((file = fopen(output_file, "a")) == NULL) {
        fprintf(stderr, "File opening failed\n");
        return; // Return if file opening fails
    }

    fputs(str, file); // Write the string to the file

    fclose(file); // Close the file
}

/* Concatenates an array of strings into a single string and saves it to a file. */
void save_line(Calc_ptr calc, char **line) {
    char str[1024] = {'\0'};

    // Concatenate each string in the array with a space separator
    for (int i = 0; line[i] != NULL; ++i) {
        strcat(str, line[i]);
        strcat(str, " ");
    }

#ifdef DEBUG
    printf("Saving line to file: %s\n", str);
#endif

    append_line(calc->out_file, str); // Append the concatenated string to the output file
}

/* Creates an error message based on the calculator's error flag and saves it to the output file. */
void save_error(Calc_ptr calc) {
    char str[128] = {'\0'};

    // Create an error message based on the calculator's error flag
    switch (calc->flag) {
        case VALID:
            break;
        case INVALID_OPERATOR:
            strcpy(str, "[err: INVALID_OPERATOR] ");
            break;
        case INVALID_BASE:
            strcpy(str, "[err: INVALID_BASE] ");
            break;
        case INVALID_NUMBER_OF_ARG:
            strcpy(str, "[err: INVALID_NUMBER_OF_ARG] ");
            break;
        case INVALID_NUMBER:
            strcpy(str, "[err: INVALID_NUMBER] ");
            break;
        case DIVISION_BY_ZERO:
            strcpy(str, "[err: DIVISION_BY_ZERO] ");
    }

    strcat(str, "\n\n");
    append_line(calc->out_file, str); // Append the error message to the output file
}

/* Checks if the input string represents a valid numerical base [2-16]*/
bool is_base(const char *str){
    size_t len = strlen(str);
    if(len == 1){
        if(str[0] >= '0' && str[0] <= '9')
            return true;
        else
            return false;
    }
    
    if(len == 2){
        if(str[0] == '1' && str[1] >= '0' && str[1] <= '6')
            return true;
        else
            return false;
    }
        

    return false;
}

/* Checks if the input string represents a valid number in the specified numerical base. */
bool is_num(const char *str, int num_base) {
    // Check if the specified numerical base is within the valid range [2, 16]
    if (num_base < 2 || num_base > 16)
        return false;

    // Skip the sign if present
    if (str[0] == '+' || str[0] == '-')
        str++;

    // Validate each character based on the specified numerical base
    for (int i = 0; str[i] != '\0'; ++i) {
        if (num_base <= 10) {
            // Check if the character is a valid digit in the specified base
            if (!(str[i] >= '0' && str[i] < '0' + num_base))
                return false;
        } else {
            // Check if the character is a valid digit or letter in the specified base
            if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'A' && str[i] <= hex_digits[num_base - 1])))
                return false;
        }
    }

    return true;
}

/* 
This function parses the input line to determine the operation type and relevant parameters.
It sets the calculator's operation, old_base, and new_base accordingly.
If an invalid operation or base is detected, it updates the calculator's flag.

Note: This function assumes that the input line is well-formed.
*/
void update_operation_status(Calc_ptr calc, char** line)
{
    calc->flag = VALID;

#ifdef DEBUG
    printf("Update_operation : Operation %d, flag %d\n", calc->operation, calc->flag);
#endif

    switch(line[0][0])
    {
        case '+':
            if(is_base(line[1])){
                calc->operation = ADD;
                calc->old_base = atoi(line[1]);
            }
            else
                calc->operation = UNDEFINED;
            
            break;

        case '-':
            if(is_base(line[1])){
                calc->operation = SUBSTRACT;
                calc->old_base = atoi(line[1]);
            }
            else
                calc->operation = UNDEFINED;
            
            break;

        case '*':
            if(is_base(line[1])){
                calc->operation = MULTIPLY;
                calc->old_base = atoi(line[1]);
            }
            else
                calc->operation = UNDEFINED;
            
            break;

        case '/':
            if(is_base(line[1])){
                calc->operation = DIVIDE;
                calc->old_base = atoi(line[1]);
            }
            else
                calc->operation = UNDEFINED;
            
            break;

        case '^':
            if(is_base(line[1])){
                calc->operation = EXPONENTIATE;
                calc->old_base = atoi(line[1]);
            }
            else
                calc->operation = UNDEFINED;
            
            break;

        case '%':
            if(is_base(line[1])){
                calc->operation = MODULO;
                calc->old_base = atoi(line[1]);
            }
            else
                calc->operation = UNDEFINED;
            
            break;

        default:
            if(is_base(line[0]) ){
                if(is_base(line[1])){
                    calc->operation = CHANGE_BASE;
                    calc->old_base = atoi(line[0]);
                    calc->new_base = atoi(line[1]);
                }
                else{
                    calc->operation = UNDEFINED;
                    calc->flag = INVALID_BASE;
                }
            }
            else
                calc->operation = UNDEFINED;
    }

    if(calc->operation == UNDEFINED &&  calc->flag == VALID)
        calc->flag = INVALID_OPERATOR;
}


/*
This function performs the specified operation using the calculator's memory and act_number.
If the operation requires more than one argument, it checks the validity of the act_number and performs the operation.
If an error occurs during the calculation, it updates the calculator's flag accordingly.

Note: This function assumes that the calculator has been properly initialized.
*/
void make_calculations(Calc_ptr calc, int *argument_counter)
{
#ifdef DEBUG
    printf("Make_calculations : Operation %d, flag %d\n", calc->operation, calc->flag);
#endif

    if(*argument_counter == 1 && calc->operation == CHANGE_BASE)
    {
        convert_system_BigNum(calc->memory, calc->new_base);
        calc->ready_result = true;
    }

    BigNum_ptr bg;

    if(*argument_counter >= 2)
    {
        switch(calc->operation)
        {
            case ADD:
                add_BigNum(calc->memory, calc->act_number);
                calc->ready_result = true;
                break;

            case SUBSTRACT:
                subtract_BigNum(calc->memory, calc->act_number);
                calc->ready_result = true;
                break;

            case MULTIPLY:
                multiply_BigNum(calc->memory, calc->act_number);
                calc->ready_result = true;
                break;

            case DIVIDE:
                bg = int_to_BigNum(0);
                convert_system_BigNum(bg, calc->old_base);
                if(compare_abs_BigNum(calc->act_number, bg) != 0){
                    divide_BigNum(calc->memory, calc->act_number);
                    calc->ready_result = true;
                }
                else{
                    calc->flag = DIVISION_BY_ZERO;
                    *argument_counter -= 1;
                }
                delete_BigNum(bg);
                break;

            case EXPONENTIATE:
                if(BigNum_sign(calc->act_number) == PLUS){
                    exponentiate_BigNum(calc->memory, calc->act_number);
                    calc->ready_result = true;
                }
                else{
                    calc->flag = INVALID_NUMBER;
                    *argument_counter -= 1;
                }
                break;

            case MODULO:
                bg = int_to_BigNum(0);
                convert_system_BigNum(bg, calc->old_base);
                if(compare_abs_BigNum(calc->act_number, bg) != 0){
                    modulo_BigNum(calc->memory, calc->act_number);
                    calc->ready_result = true;
                }
                else{
                    calc->flag = DIVISION_BY_ZERO;
                    *argument_counter -= 1;
                }
                delete_BigNum(bg);
                break;

            case CHANGE_BASE:
                calc->flag = INVALID_NUMBER_OF_ARG;
                *argument_counter -= 1;
                break;

            default:
                fprintf(stderr, "Make_calculation: UNDEFINED_OPERATION\n");
        }
    }
    
}

/*
This function reads lines from the input file, processes them, and performs calculations accordingly.
It updates the calculator's operation, flag, and performs the specified calculations using the make_calculations function.
The results are saved to the output file.

Description:
For each line in the input file, the function updates the calculator's status, processes the line, and saves the results.
If an error occurs during processing, it updates the calculator's flag and saves the error to the output file.

Note: This function assumes that the calculator has been properly initialized.
*/
void start_calculations(Calc_ptr calc)
{
    printf("            _            _       _             \n");
    printf("           | |          | |     | |            \n");
    printf("   ___ __ _| | ___ _   _| | __ _| |_ ___  _ __ \n");
    printf("  / __/ _` | |/ __| | | | |/ _` | __/ _ \\| '__|\n");
    printf(" | (_| (_| | | (__| |_| | | (_| | || (_) | |   \n");
    printf("  \\___\\__,_|_|\\___|\\__,_|_|\\__,_|\\__\\___/|_|   \n");
    printf(" ==============================================\n\n");
    printf("Performing operations...\n");


    char **line = load_line(calc->in_file, &calc->index);

    int argument_counter = 0;

    while(line != NULL)
    {
        int n = words_in_line(line);

#ifdef DEBUG
        printf("SC(WHILE): Operation %d, flaga %d\n", calc->operation, calc->flag);
#endif
        switch(n)
        {
            case 1:
                if(calc->operation == UNDEFINED){
                    calc->flag = INVALID_OPERATOR;
                    break;
                }

                if(is_num(line[0], calc->old_base))
                {
                    calc->flag = VALID;
                    if(argument_counter == 0){
                        assign_value(calc->memory, line[0], calc->old_base);
                        zero_justify(calc->memory);
                    }
                    else{
                        assign_value(calc->act_number, line[0], calc->old_base);
                        zero_justify(calc->act_number);
                    }
                    ++argument_counter;

                    make_calculations(calc, &argument_counter);

                }
                else
                {
                    calc->flag = INVALID_NUMBER;
                }

#ifdef DEBUG
                printf("SC(case1): Operation %d, flaga %d\n", calc->operation, calc->flag);
#endif
                break;
            case 2:
                if(calc->ready_result){
#ifdef DEBUG
                    printf("SC(case2): Saving calculated value from calc->memory to file\n");
#endif
                    char *str = BigNum_to_cstring(calc->memory);
                    append_line(calc->out_file, str);
                    append_line(calc->out_file, "\n--------------------------------------------------------------\n\n");
                    free(str);
                    calc->ready_result = false;
                }
                else if(argument_counter != 0){
                    calc->flag = INVALID_NUMBER_OF_ARG;
                    save_error(calc);
                    append_line(calc->out_file, "--------------------------------------------------------------\n\n");
                }
                else if(calc->flag != VALID)
                {
                    save_error(calc);
                    append_line(calc->out_file, "--------------------------------------------------------------\n\n");
                }

                update_operation_status(calc, line);
                argument_counter = 0;

#ifdef DEBUG
                printf("SC(case2): Operation %d, flaga %d\n", calc->operation, calc->flag);
#endif
                break;
            default:
                calc->flag = INVALID_NUMBER_OF_ARG;
#ifdef DEBUG
                printf("SC(case3): Operation %d, flaga %d\n", calc->operation, calc->flag);
#endif
        }

#ifdef DEBUG
        for(int i = 0; line[i] != NULL; ++i)
                printf("%s ", line[i]);

            printf("\n");
#endif
        
        save_line(calc, line);
        save_error(calc);


        line_memory_deallocation(line);
        line = load_line(calc->in_file, &calc->index);
    }


    if(calc->ready_result){
        char *str = BigNum_to_cstring(calc->memory);
        append_line(calc->out_file, str);
        free(str);
    }
    else if(calc->flag == VALID){
        calc->flag = INVALID_NUMBER_OF_ARG;
        save_error(calc);
    }
    
    append_line(calc->out_file, "--------------------------------------------------------------\n\n");

    line_memory_deallocation(line);

    printf("Calculations completed\n");
    printf(" ==============================================\n");
}