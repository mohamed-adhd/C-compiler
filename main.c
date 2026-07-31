#include <c++/16.1.1/cstdio>
#include <stdio.h>
#include <stdlib.h>
#include "lexer/lexer.h"
// 1. read source file (e.g. from argv[1]) into a string
// 2. lexer_init + loop calling next_token until TOKEN_EOF, collect into a token array
// 3. parser_init with that token array
// 4. astnode *ast = parse_program(&p);
// 5. FILE *out = fopen("output.s", "w");
// 6. codegen_program(ast, out);
// 7. fclose(out);
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
    token temp =next_token(&lex_luther);
    while (temp.type!=TOKEN_EOF) {
        
    }

    return 0;
}
