#include <stdio.h>
#include <stdlib.h>

#include "util.h"

char* readFile(const char* filename)
{
    FILE* fp;
    long size = 0;
    char* content;

    /* Read File to get size */
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Couldn't open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp) + 1;
    fclose(fp);

    /* Read File for Content */
    fp = fopen(filename, "r");
    content = malloc(size);
    fread(content, 1, size - 1, fp);
    content[size - 1] = '\0';
    fclose(fp);

    return content;
}

void initialize(DynamicArray* dynArray, size_t initialCapacity)
{
    dynArray->array = (float*)malloc(initialCapacity * sizeof(float));
    dynArray->size = 0;
    dynArray->capacity = initialCapacity;
}

void push(DynamicArray* dynArray, float value)
{
    if (dynArray->size == dynArray->capacity) {
        // Double the capacity
        dynArray->capacity *= 2;
        dynArray->array = (float*)realloc(dynArray->array, dynArray->capacity * sizeof(float));
    }
    dynArray->array[dynArray->size++] = value;
}

void cleanup(DynamicArray* dynArray)
{
    free(dynArray->array);
    dynArray->array = NULL;
    dynArray->size = dynArray->capacity = 0;
}
