//
// Created by bro on 8/4/26.
//
#include <stdio.h>
#include <stdlib.h>
#include "masm.h"

#include <ctype.h>
#include <string.h>

void lexer_init(asm_lexer *l, const char *source) {
    l->source = source;
    l->position = 0;
}
char peek(asm_lexer *l) {
    return l->source[l->position];
}
char advance(asm_lexer *l) {
    l->position++;
    return l->source[l->position-1];
}
void skip_whitespace(asm_lexer *l) {
    for (;;) {
        while (isspace((unsigned char)l->source[l->position])) {
            l->position++;
        }
        break;
    }
}
asm_token asm_next_token(asm_lexer *l) {
    asm_skip_whitespace(l);
    asm_token temp = {0};
    char s = l->source[l->position];
    if (s == '\0') {
        temp.type = ASM_TOKEN_EOF;
        return temp;
    }

    if (s==','||s=='['||s==']'||s=='.'||s==':'||s=='\n') {
        switch (s) {
            case ':':
                temp.type = ASM_TOKEN_COLON;
                break;
            case '[':
                temp.type = ASM_TOKEN_LBRACE;
                break;
            case ']':
                temp.type = ASM_TOKEN_RBRACE;
                break;
            case ',':
                temp.type = ASM_TOKEN_COMMA;
                break;
            case '\n':
                temp.type = ASM_TOKEN_NEWLINE;
                break;
            case '.':
                temp.type = ASM_TOKEN_POINT;
                break;
        }
        l->position++;
        return temp;
    } else if (isalpha((unsigned char)s) || s == '_' || s == '.') {
        char buffer[64];
        int i = 0;
        while (isalnum((unsigned char)l->source[l->position]) ||
               l->source[l->position] == '_' ||
               l->source[l->position] == '.') {
            buffer[i++] = l->source[l->position];
            l->position++;}
        buffer[i] = '\0';

        temp.type = ASM_TOKEN_IDENTIFIER;
        temp.txt = strdup(buffer);
        return temp;
    }
}
void asm_free_tokens(asm_token *tokens) {
    if (tokens == NULL) {
        return;
    }for (int i = 0; tokens[i].type != ASM_TOKEN_EOF; i++) {
        free(tokens[i].txt);
    }
    free(tokens);
}
asm_token *tokenizer(asm_lexer *l, int *count) {
    int capacity = 16;
    asm_token *tokens = malloc((size_t)capacity * sizeof(asm_token));
    if (tokens == NULL) {
        *count = 0;
        return NULL;
    }
    int i = 0;
    do {
        if (i == capacity) {
            capacity *= 2;
            asm_token *new_tokens = realloc(tokens, (size_t)capacity * sizeof(asm_token));
            if (new_tokens == NULL) {
                asm_free_tokens(tokens);
                *count = 0;
                return NULL;
            }
            tokens = new_tokens;
        }
        tokens[i] = asm_next_token(l);
    } while (tokens[i++].type != ASM_TOKEN_EOF);
    *count = i;
    return tokens;
}


