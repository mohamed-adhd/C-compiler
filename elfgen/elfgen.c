//
// Created by bro on 8/3/26.
//
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
void write_u16(unsigned char *buf, int offset, unsigned short value) {
    buf[offset]   = value &  0xFF;
    buf[offset+1] = (value >> 8) & 0xFF;
}

void write_u32(unsigned char *buf, int offset, unsigned int value) {
    buf[offset]   = value & 0xFF;
    buf[offset+1] = (value >> 8)  & 0xFF;
    buf[offset+2] = (value >> 16) & 0xFF;
    buf[offset+3] = (value >> 24) & 0xFF;
}

void write_u64(unsigned char *buf, int offset, unsigned long value) {
    for (int i = 0; i < 8; i++) {
        buf[offset + i] = (value >> (8 * i)) & 0xFF;
    }
}
void generate() {
    FILE *file = fopen("output.bin", "rb");
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
    offset[16]=2;
    offset[17]=0;
    offset[18]=62;
    offset[19]=0;
    offset[20]=1;
    offset[21]=0;
    offset[22]=0;
    offset[23]=0;
    write_u64(offset, 24, 0x400000+120);
    write_u64(offset, 32,64);
    write_u64(offset, 40,0);
    write_u32(offset, 48, 0);
    write_u16(offset, 52,64);
    write_u16(offset, 54, 56);
    write_u16(offset, 56, 1);
    write_u16(offset, 58, 0);
    write_u16(offset, 60, 0);
    write_u16(offset, 62, 0);
    write_u32(offset, 64, 1);
    write_u32(offset, 68, 7);
    write_u64(offset, 72, 120);
    write_u64(offset, 80, 0x400000+120);
    write_u64(offset, 88, 0x400000+120);
    write_u64(offset,96,file_size + 120);
    write_u64(offset,104,file_size + 120);
    write_u64(offset, 112, 0x1000);
    uint8_t *final = (uint8_t *) malloc(120+file_size);
    if (!final) {
        fprintf(stderr, "ay malloc fucked us up , jump his ass\n");
        exit(1);
    }
    memcpy(final,offset,120);
    memcpy(final+120,buff,file_size);
    FILE* so= fopen("typeshi","wb");
        fwrite(final,1,file_size+120,so);
        chmod("typeshi", 0755);
    fclose(so);


}

