#ifndef VECTOR
#define VECTOR

#include <stdlib.h>
#include <stdbool.h>

/* Type definition for the elements stored in the vector */
#define DATA_TYPE unsigned char

/* Type definition for a NULL value in the vector */
#define NULL_TYPE NULL

/* Structure to represent a dynamic array (vector) */
typedef struct {
    size_t size;      /* Number of elements in the vector */
    size_t capacity;  /* Current capacity of the vector */
    DATA_TYPE *array; /* Pointer to the dynamic array */
} Vector;

/* Defining Vector_ptr as a pointer to a Vector structure */
typedef Vector *Vector_ptr;

/*
    Function: create_vector
    -----------------------
    Allocates memory for a new vector and initializes its properties.

    Returns:
    - A pointer to the newly created vector on success.
    - NULL_TYPE if memory allocation fails during vector creation.
*/
Vector_ptr create_vector();

/*
    Function: delete_vector
    -----------------------
    Deletes a vector and frees the associated memory.

    Parameters:
    - ptr: A pointer to the vector to be deleted.

    Returns:
    - NULL_TYPE after freeing the vector's memory.
*/
Vector_ptr delete_vector(Vector_ptr ptr);

/*
    Function: copy_vector
    ---------------------
    Creates a new vector, resizes it, and copies elements from the input vector.

    Parameters:
    - ptr: A pointer to the vector to be copied.

    Returns:
    - A pointer to the newly created vector on success.
    - NULL_TYPE if the copy operation fails due to memory allocation issues.
*/
Vector_ptr copy_vector(Vector_ptr ptr);

/*
    Function: reverse_vector
    ------------------------
    Reverses the order of elements in the vector.

    Parameters:
    - ptr: A pointer to the vector to be reversed.

    Returns:
    - A pointer to the reversed vector on success.
    - NULL_TYPE if the input vector is invalid.
*/
Vector_ptr reverse_vector(Vector_ptr ptr);

/*
    Function: vector_reserve
    -----------------------
    Resizes the vector to a new specified capacity.
    If memory allocation success, vector capacity is changed,
    otherwise vector remains unchanged.

    Parameters:
    - ptr: A pointer to the vector to be resized.
    - new_capacity: The new capacity for the vector.
*/
void vector_reserve(Vector_ptr ptr, size_t new_capacity);

/*
    Function: vector_resize
    -----------------------
    Resizes the container so that it contains new_size elements.
    If new_size < size, the content is reduced to its first n elements, removing those beyond
    If new_size > size, the content is expanded by inserting at the end as many elements (0's) as needed to reach a size of n.
    If new_size > capacity, an automatic reallocation of the allocated storage space takes place.

    Parameters:
    - ptr: A pointer to the vector to be resized.
    - new_size: The new size for the vector.
*/
void vector_resize(Vector_ptr ptr, size_t new_size);

/*
    Function: vector_clear
    ----------------------
    Removes all elements from the vector while keeping the capacity unchanged.

    Parameters:
    - ptr: A pointer to the vector to be cleared.
*/
void vector_clear(Vector_ptr ptr);

/*
    Function: push_back
    -------------------
    Adds an element to the end of the vector. Doubles capacity if the vector is full.
    Calls vector_resize function, if operation faills push_back isn't performed.
    
    Parameters:
    - ptr: A pointer to the vector.
    - val: The value to be added to the vector.
*/
void push_back(Vector_ptr ptr, DATA_TYPE val);

/*
    Function: pop_back
    ------------------
    Removes the last element from the vector.

    Parameters:
    - ptr: A pointer to the vector.
*/
void pop_back(Vector_ptr ptr);

/*
    Function: vector_front
    ----------------------
    Gets the value of the first element in the vector.

    Parameters:
    - ptr: A pointer to the vector.

    Returns:
    - The value of the first element.
    - -1 if the vector is empty or invalid.
*/
DATA_TYPE vector_front(Vector_ptr ptr);

/*
    Function: vector_back
    ---------------------
    Gets the value of the last element in the vector.

    Parameters:
    - ptr: A pointer to the vector.

    Returns:
    - The value of the last element.
    - -1 if the vector is empty or invalid.
*/
DATA_TYPE vector_back(Vector_ptr ptr);

/*
    Function: vector_get
    ---------------------
    Gets the value of an element at a specific index in the vector.

    Parameters:
    - ptr: A pointer to the vector.
    - index: The index of the element to retrieve.

    Returns:
    - The value of the element at the specified index.
    - -1 if the vector is invalid or the index is out of bounds.
*/
DATA_TYPE vector_get(Vector_ptr ptr, size_t index);

/*
    Function: vector_set
    ---------------------
    Sets the value of an element at a specific index in the vector.

    Parameters:
    - ptr: A pointer to the vector.
    - index: The index of the element to be set.
    - val: The value to set.
*/
void vector_set(Vector_ptr ptr, size_t index, DATA_TYPE val);

/*
    Function: vector_size
    ---------------------
    Gets the current number of elements in the vector.

    Parameters:
    - ptr: A pointer to the vector.

    Returns:
    - The current number of elements in the vector.
    - 0 if the vector is invalid.
*/
size_t vector_size(Vector_ptr ptr);

/*
    Function: vector_capacity
    -------------------------
    Gets the current capacity of the vector.

    Parameters:
    - ptr: A pointer to the vector.

    Returns:
    - The current capacity of the vector.
    - 0 if the vector is invalid.
*/
size_t vector_capacity(Vector_ptr ptr);

/*
    Function: vector_empty
    -----------------------
    Checks if the vector is empty (contains no elements).

    Parameters:
    - ptr: A pointer to the vector.

    Returns:
    - true if the vector is empty or invalid.
    - false otherwise.
*/
bool vector_empty(Vector_ptr ptr);

/*
    Function: print_vector
    ----------------------
    Prints the elements of the vector to the standard output in the format [element0, element1, ..., element(n-1)].

    Parameters:
    - ptr: A pointer to the vector.
*/
void print_vector(Vector_ptr ptr);

#endif