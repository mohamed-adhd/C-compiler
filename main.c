#include <c++/16.1.1/cstdio>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    
    FILE *file = fopen("example.txt", "r");
        if (file == NULL) {
            printf("Can't open file!\n");
            return 1;
        }
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *content = malloc(size + 1);
        fread(content, 1, size, file);
        content[size] = '\0';
        fclose(file);

        return 0;
    }



    return 0;
}
char* readFileToString(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error!\n");
        return NULL;
    }
}
