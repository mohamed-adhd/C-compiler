#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"
#include "parse.h"
#include "token.h"
#include "lexer.h"

#include <stddef.h>

int main(int argc, char *argv[]) {

    const char *path = argc > 1 ? argv[1] : "../tests/test2.c";
    FILE *file = fopen(path, "r");
    if (file == NULL && argc == 1) {
        path = "tests/test1.c";
        file = fopen(path, "r");
    }
    if (file ==NULL) {
        printf("Can't open %s\n", path);
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
    int token_count = 0;
    token *tkr = tokenizer(&lex_luther, &token_count);
    paarser psr;
    parser_init(&psr,tkr,token_count);
    astnode *ast = parse_program(&psr);
    FILE *out = fopen("output.s", "w");
    codegen_program(ast, out);


    const char *a= "mov rax, 60\n"
    "mov rdi, 0\n"
    "syscall\n";
    fprintf(out, "%s",a);
    fclose(out);
    free_tokens(tkr);
    free(content);
    system("nasm -f elf64 output.s -o output.o");
    system("ld output.o -o output");
    system("./output");
    return 0;

}
