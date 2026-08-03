//
// Created by bro on 8/3/26.
//

#ifndef C_COMPILER_ELFGEN_H
#define C_COMPILER_ELFGEN_H
#include <stdint.h>
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
    uint8_t *offset = (uint8_t *) malloc(120*sizeof(uint8_t));
    offset[0]=0x7F;
    offset[1]='E';
    offset[2]='L';
    offset[3]='F';
    offset[4]=2;
    offset[5]=1;
    offset[6]=1;
    offset[7]=0;
    offset[8]=0;
    offset[9]=0;
    offset[10]=0;
    offset[11]=0;
    offset[12]=0;
    offset[13]=0;
    offset[14]=0;
    offset[15]=0;
    offset[16]=0;

}

#endif //C_COMPILER_ELFGEN_H
