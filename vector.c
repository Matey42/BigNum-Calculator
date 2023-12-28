#include "vector.h"
#include <stdio.h>

/* Function allocates memory for a new vector, initializes its size,
 capacity, and array pointer. If memory allocation fails, 
 it print an error message to stderr and returns NULL_TYPE. */
Vector_ptr create_vector()
{
    Vector_ptr vec = (Vector_ptr)malloc(sizeof(Vector));
    if(vec == NULL){
        fprintf(stderr, "Memory allocation failed during vector creation\n");
        return NULL_TYPE;
    }

    vec->size = 0;
    vec->capacity = 0;
    vec->array = NULL_TYPE;

    return vec;
}

/* Deletes a vector and frees the memory allocated for it.
 It first checks if the input vector is not NULL, frees the array if it exists,
 and then frees the vector itself. Returns NULL_TYPE */
Vector_ptr delete_vector(Vector_ptr ptr)
{
    if(ptr != NULL){
        if(ptr->array != NULL)
            free(ptr->array);
        free(ptr);
    }

    return NULL_TYPE;
}

/* Creates a new vector, resize it and copies the elements from the input vector
 to the new one. It checks if all operation have been performed successful by comparing the 
 capacities of the two vectors (vector_resize doesn't change vector capacity if sth went wrong).
 If unsuccessful, it prints an error message to stderr and returns NULL_TYPE */
Vector_ptr copy_vector(Vector_ptr ptr)
{
    Vector_ptr vec = create_vector();
    vector_reserve(vec, vector_capacity(ptr));
    
    if(vec->capacity != ptr->capacity){
        fprintf(stderr, "Copy failed.\n");
        return NULL_TYPE;
    }

    for(size_t i = 0; i < vector_size(ptr); ++i)
        push_back(vec, vector_get(ptr,i));

    return vec;
}

/* This is a utility function to swap two elements.
  Used in teverse vector */
void element_swap(DATA_TYPE *el1, DATA_TYPE *el2)
{
    DATA_TYPE tmp = *el1;
    *el1 = *el2;
    *el2 = tmp;
}

/* Function reverses the elements of the vector.
It uses the element_swap utility function to swap elements from the beginning to the middle of the vector.*/
Vector_ptr reverse_vector(Vector_ptr ptr)
{
    if(ptr == NULL){
        fprintf(stderr, "Invalid vector\n");
        return NULL_TYPE;
    }

    size_t mid = vector_size(ptr) / 2;

    for(size_t i = 0; i < mid; ++i)
        element_swap(&ptr->array[i], &ptr->array[vector_size(ptr) - i - 1]);

    return ptr;
}

/* Function resizes the vector to a new specified capacity. It checks if the new capacity is valid
 and reallocates memory for the array. If memory allocation success, vector capacity is changed.
 Otherwise, it prints an error message to stderr and leaves vector unchanged.*/
void vector_reserve(Vector_ptr ptr, size_t new_capacity)
{
    if(ptr == NULL || new_capacity < ptr->size){
        fprintf(stderr, "Invalid vector or invalid new capacity\n");
        return;
    }

    DATA_TYPE* array;

    if( (array = (DATA_TYPE*)realloc(ptr->array, new_capacity * sizeof(DATA_TYPE))) == NULL_TYPE){
        fprintf (stderr, "Memory allocation failed during vector resize\n");
        return;
    }
    
    ptr->array = array;
    ptr->capacity = new_capacity; 
}

/* Function resizes vector to a new specified size. If new_size > size the content is expanded by inserting 0's at the end to reach a new_size 
  If new_size < size than the content is reduced to its first new_size elements. */
void vector_resize(Vector_ptr ptr, size_t new_size)
{
    if(ptr == NULL ){
        fprintf(stderr, "Invalid vector or invalid new capacity\n");
        return;
    }

    if(new_size > ptr->capacity){
        vector_reserve(ptr, new_size);
    }
    
    for(size_t i = ptr->size; i < new_size; ++i)
        push_back(ptr, 0);
}

/* Removes all elements from the vector while keeping the capacity unchanged
   by simply setting the size to 0. */
void vector_clear(Vector_ptr ptr)
{
    if(ptr == NULL)
        fprintf(stderr, "Invalid vector\n");
        
    ptr->size = 0;
}

/*  Function adds an element to the end of the vector. If the vector is full, 
 it doubles its capacity using vector_resize before adding the new element. */
void push_back(Vector_ptr ptr, DATA_TYPE val)
{
    if(ptr == NULL){
        fprintf(stderr, "Invalid vector\n");
        return;
    }

    if(ptr->size >= ptr->capacity){ 
        size_t new_capacity = (ptr->capacity == 0) ? 2 : ptr->capacity * 2;
        vector_reserve(ptr, new_capacity);

        if(ptr->capacity < new_capacity)
        {
            fprintf(stderr, "Push_back failed.\n");
            return;
        }
    }

    ptr->array[ptr->size] = val;
    ptr->size++;
}

/*  Removes the last element from the vector by decrementing its size.
 It checks if the vector is empty before attempting to pop an element.
 If operation invalid -> prints an error message to stderr*/
void pop_back(Vector_ptr ptr)
{
    if(ptr == NULL || ptr->size == 0){
        fprintf(stderr, "Pop_back func: Invalid or empty vector\n");
        return;
    }

    ptr->size--;
}

/* Returns the value of the first element in the vector.
 It checks if the vector is empty before accessing the array. 
 If operation invalid -> prints an error message to stderr and returns -1*/
DATA_TYPE vector_front(Vector_ptr ptr)
{
    if(ptr == NULL || ptr->size == 0){
        fprintf(stderr, "Vector_front: Invalid or empty vector\n");
        return -1;
    }

    return ptr->array[0];
}

/* Returns the value of the last element in the vector.
 It checks if the vector is empty before accessing the array. 
 If operation invalid -> prints an error message to stderr and returns -1*/
DATA_TYPE vector_back(Vector_ptr ptr)
{
    if(ptr == NULL || ptr->size == 0){
        fprintf(stderr, "Vector_back: Invalid or empty vector\n");
        return -1;
    }

    return ptr->array[ptr->size - 1];
}

/*  Returns the value of an element at a specific index in the vector.
 It checks if the index is within bounds before accessing the array.
 If operation invalid -> prints an error message to stderr and returns -1*/
DATA_TYPE vector_get(Vector_ptr ptr, size_t index)
{
    if(ptr == NULL || index >= ptr->size){
        fprintf(stderr, "Invalid vector or out-of-bounds index\n");
        return -1;
    }

    return ptr->array[index];
}

/* Sets the value of an element at a specific index in the vector.
 It checks if the index is within bounds before modifying the array.
 If operation invalid -> prints an error message to stderr */
void vector_set(Vector_ptr ptr, size_t index, DATA_TYPE val)
{
    if(ptr == NULL || index >= ptr->size){
        fprintf(stderr, "Invalid vector or out-of-bounds index\n");
        return;
    }

    ptr->array[index] = val;
}

/*  Returns the current number of elements in the vector. */
size_t vector_size(Vector_ptr ptr)
{
    if(ptr == NULL){
        fprintf(stderr, "Invalid vector\n");
        return 0;
    }

    return ptr->size;
}

/* Returns the current capacity of the vector. 
 (Maximum number of elements that can be stored in vector 
  before the need to perform vector_resize operation) */
size_t vector_capacity(Vector_ptr ptr)
{
    if(ptr == NULL){
        fprintf(stderr, "Invalid vector\n");
        return 0;
    }

    return ptr->capacity;
}

/* Function returns true if the vector is empty (contains no elements) or Vector_ptr is NULL; 
 otherwise, it returns false.. */
bool vector_empty(Vector_ptr ptr){
    return (ptr == NULL || ptr->size == 0);
}

/* Prints the elements of the vector to the standard output in the format 
  [element0, element1, ..., element(n-1)]. */
void print_vector(Vector_ptr ptr)
{
    if(ptr == NULL){
        fprintf(stderr, "Invalid vector\n");
        return ;
    }

    if(vector_size(ptr) == 0){
        printf("[]\n");
    }
    else{
        printf("[%u", vector_front(ptr));

        for(size_t i = 1; i < vector_size(ptr); ++i)
        {
            printf(", %u", vector_get(ptr, i));
        }
        printf("]\n");
    }
}

