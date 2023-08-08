
#ifndef __UTIL_H__
#define __UTIL_H__

#include "stddef.h"

char* readFile(const char* filename);

typedef struct {
    float* array;
    size_t size;
    size_t capacity;
} DynamicArray;

void initialize(DynamicArray* dynArray, size_t initialCapacity);
void push(DynamicArray* dynArray, float value);
void cleanup(DynamicArray* dynArray);

#endif