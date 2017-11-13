#ifndef _orderedArray_h
#define _orderedArray_h

#include <stdint.h>

typedef void* type_t;

typedef int8_t (*lessThanPredicate_t)(type_t, type_t);

typedef struct orderedArray {
    type_t* array;
    uint32_t size;
    uint32_t maxSize;
    lessThanPredicate_t lessThan;
} orderedArray_t;

int8_t standardLessThanPredicate(type_t a, type_t b);

orderedArray_t createOrderedArray(uint32_t maxSize, lessThanPredicate_t lessThan);
orderedArray_t placeOrderedArray(void* address, uint32_t maxSize, lessThanPredicate_t lessThan);
void destroyOrderedArray(orderedArray_t* array);
void insertOrderedArray(orderedArray_t* array, type_t element);
void lookupOrderedArray(uint32_t index, orderedArray_t* array);
void removeOrderedArray(uint32_t index, orderedArray_t* array);

#endif