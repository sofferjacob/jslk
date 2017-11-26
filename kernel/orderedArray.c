// ordered_array.c -- Implementation for creating, inserting and deleting
//                    from ordered arrays.
//                    Written for JamesM's kernel development tutorials.

#include <orderedArray.h>
#include <assert.h>

int8_t standard_lessthan_predicate(type_t a, type_t b)
{
    return (a < b) ? 1 : 0;
}

orderedArray_t createOrderedArray(uint32_t max_size, lessthan_predicate_t less_than)
{
    orderedArray_t to_ret;
    to_ret.array = (void *)kmalloc(max_size * sizeof(type_t));
    memset(to_ret.array, 0, max_size * sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

orderedArray_t placeOrderedArray(void *addr, uint32_t max_size, lessthan_predicate_t less_than)
{
    orderedArray_t to_ret;
    to_ret.array = (type_t *)addr;
    memset(to_ret.array, 0, max_size * sizeof(type_t));
    to_ret.size = 0;
    to_ret.max_size = max_size;
    to_ret.less_than = less_than;
    return to_ret;
}

void destroyOrderedArray(orderedArray_t *array)
{
    //    kfree(array->array);
}

void insertOrderedArray(orderedArray_t *array, type_t item)
{
    assert(array->less_than);
    uint32_t iterator = 0;
    while (iterator < array->size && array->less_than(array->array[iterator], item))
        iterator++;
    if (iterator == array->size) // just add at the end of the array.
        array->array[array->size++] = item;
    else
    {
        type_t tmp = array->array[iterator];
        array->array[iterator] = item;
        while (iterator < array->size)
        {
            iterator++;
            type_t tmp2 = array->array[iterator];
            array->array[iterator] = tmp;
            tmp = tmp2;
        }
        array->size++;
    }
}

type_t lookupOrderedArray(uint32_t i, orderedArray_t *array)
{
    assert(i < array->size);
    return array->array[i];
}

void removeOrderedArray(uint32_t i, orderedArray_t *array)
{
    while (i < array->size)
    {
        array->array[i] = array->array[i + 1];
        i++;
    }
    array->size--;
}
