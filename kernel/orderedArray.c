#include <stdint.h>
#include <orderedArray.h>
#include <hal.h>
#include <assert.h>

int8_t standardLessThanPredicate(type_t a, type_t b) {
    return (a < b)?1:0;
}

orderedArray_t createOrderedArray(uint32_t maxSize, lessThanPredicate_t lessThan) {
    orderedArray_t tmp;
    tmp.array = kmalloc(sizeof(type_t)*maxSize);
    memset(tmp.array, 0, sizeof(type_t)*maxSize);
    tmp.size = 0;
    tmp.maxSize = maxSize;
    tmp.lessThan = lessThan;
    return tmp;
}

orderedArray_t createOrderedArray(void* address, uint32_t maxSize, lessThanPredicate_t lessThan) {
    orderedArray_t tmp;
    tmp.array = (type_t*)address;
    memset(tmp.array, 0, sizeof(type_t) * maxSize);
    tmp.size = 0;
    tmp.maxSize = maxSize;
    tmp.lessThan = lessThan;
    return tmp;
}

void destroyOrderedArray(orderedArray_t* array) {
    // kfree(array->array);
}

void insertOrderedArray(orderedArray_t* array, type_t element) {
    assert(array->lessThan);
    uint32_t iterator = 0;
    while (iterator < array->size && array->lessThan(array->array[iterator], item)) {
        iterator++;
    }
    if (iterator == array->size) {
        array->size++;
    } else {
        type_t tmp = array->array[iterator];
        array->array[iterator] = item;
        while (iterator < array->size) {
           iterator++;
           type_t tmp2 = array->array[iterator];
           array->array[iterator] = tmp;
           tmp = tmp2;
        }
        array->size++;
    }
}

type_t lookupOrderedArray(uint32_t index, orderedArray_t* array) {
    assert(index < array->size);
    return array->array[index];
}

void removeOrderedArray(uint32_t index, orderedArray_t *array) {
       while (i < array->size) {
               array->array[i] = array->array[i + 1];
               i++;      
      }
      array->size--;
}
