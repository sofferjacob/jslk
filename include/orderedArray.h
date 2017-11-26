// ordered_array.h -- Interface for creating, inserting and deleting
//                    from ordered arrays.
//                    Written for JamesM's kernel development tutorials.

#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

#include <stdint.h>

/**
   This array is insertion sorted - it always remains in a sorted state (between calls).
   It can store anything that can be cast to a void* -- so a uint32_t, or any pointer.
**/
typedef void *type_t;
/**
   A predicate should return nonzero if the first argument is less than the second. Else 
   it should return zero.
**/
typedef int8_t (*lessthan_predicate_t)(type_t, type_t);
typedef struct
{
    type_t *array;
    uint32_t size;
    uint32_t max_size;
    lessthan_predicate_t less_than;
} orderedArray_t;

/**
   A standard less than predicate.
**/
int8_t standard_lessthan_predicate(type_t a, type_t b);

/**
   Create an ordered array.
**/
orderedArray_t createOrderedArray(uint32_t max_size, lessthan_predicate_t less_than);
orderedArray_t placeOrderedArray(void *addr, uint32_t max_size, lessthan_predicate_t less_than);

/**
   Destroy an ordered array.
**/
void destroyOrderedArray(orderedArray_t *array);

/**
   Add an item into the array.
**/
void insertOrderedArray(orderedArray_t *array, type_t item);

/**
   Lookup the item at index i.
**/
type_t lookupOrderedArray(uint32_t i, orderedArray_t *array);

/**
   Deletes the item at location i from the array.
**/
void removeOrderedArray(uint32_t i, orderedArray_t *array);

#endif // ORDERED_ARRAY_H
