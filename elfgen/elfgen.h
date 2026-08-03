//
// Created by bro on 8/3/26.
//

#ifndef C_COMPILER_ELFGEN_H
#define C_COMPILER_ELFGEN_H
#include <stdio.h>
#include <stdlib.h>

void generate() {
    FILE *file = fopen("output.o", "rb");
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    uint8_t *buff = (uint8_t *) malloc(file_size);
    size_t bytes_read = fread(buff, 1, file_size, file);
    fclose(file);
}

#endif //C_COMPILER_ELFGEN_H
