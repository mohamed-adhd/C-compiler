#include <c++/16.1.1/cstdio>
#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"
#include "parse.h"
#include "lexer/lexer.h"
int main(int argc, char *argv[]) {

    FILE *file = fopen("example.txt", "r");
    if (file == NULL) {
        printf("Can't open shi\n");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);
    lexer lex_luther;
    lexer_init(&lex_luther,content);
    token *tkr = malloc(size + 1);
    token temp =next_token(&lex_luther);
    int i=1;
    while (temp.type!=TOKEN_EOF) {
        temp =next_token(&lex_luther);
        tkr[i] = temp;
    }
    paarser psr;
    parser_init(&psr,tkr,size);
    astnode *ast = parse_program(&psr);
    FILE *out = fopen("output.s", "w");
    codegen_program(ast, out);
    fclose(out);


    return 0;
}
