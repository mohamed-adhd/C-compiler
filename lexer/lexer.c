#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void lexer_init(lexer* l, const char* source){
    l->line=1;
    l->column=1;
    l->source=source;
    l->position=0;
}
char peek(lexer *l) {
    return l->source[l->position];
}

char advance(lexer *l) {
    l->position++;
    return l->source[l->position-1];
}
void skip_whitespace(lexer *l) {
    for (;;) {
        while (isspace((unsigned char)l->source[l->position])) {
            l->position++;
        }
        if (l->source[l->position] == '#') {
            while (l->source[l->position] != '\0' && l->source[l->position] != '\n') {
                l->position++;
            }
            continue;
        }
        if (l->source[l->position] == '/' && l->source[l->position + 1] == '/') {
            while (l->source[l->position] != '\0' && l->source[l->position] != '\n') {
                l->position++;
            }
            continue;
        }
        break;
    }
}

static char *copy_text(const char *start, size_t length) {
    char *text = malloc(length + 1);
    if (text == NULL) {
        return NULL;
    }
    memcpy(text, start, length);
    text[length] = '\0';
    return text;
}

token next_token(lexer *l) {
    skip_whitespace(l);

    token temp = {0};
    char s=l->source[l->position];
    if (s == '\0') {
        temp.type = TOKEN_EOF;
        return temp;
    }

    if (s==';'||s=='('||s==')'||s=='{'||s=='}'||s=='+'||s=='-'||s=='*'||s=='/'||s==','||s=='%'||s=='"') {
        switch (s) {
            case ';':
                temp.type=TOKEN_SEMICOLON;
                break;
            case '(':
                temp.type=TOKEN_LPAREN;
                break;
            case ')':
                temp.type=TOKEN_RPAREN;
                break;
            case '{':
                temp.type=TOKEN_LBRACE;
                break;
            case '}':
                temp.type=TOKEN_RBRACE;
                break;
            case '+':
                temp.type=TOKEN_ADD;
                break;
            case '-':
                temp.type=TOKEN_SUB;
                break;
            case '*':
                temp.type=TOKEN_MUL;
                break;
            case '/':
                temp.type=TOKEN_DIV;
                break;
            case ',':
                temp.type=TOKEN_ver;
                break;
            case '%':
                temp.type=TOKEN_MOD;
                break;
            case '"':
                temp.type=TOKEN_COL;
                break;
        }
        l->position++;
        return temp;
    }else if (isdigit(s)){
        int value = 0;
        while (isdigit((unsigned char)l->source[l->position])) {
            value = value * 10 + (l->source[l->position] - '0');
            l->position++;
        }
        temp.type=TOKEN_NUMBER;
        temp.val=value;
        return temp;
    }else if (isalpha((unsigned char)s) || s == '_') {
        size_t start = l->position;
        while (isalnum((unsigned char)l->source[l->position]) || l->source[l->position] == '_') {
            l->position++;
        }
        size_t length = l->position - start;
        char *word = copy_text(l->source + start, length);
        temp.txt = word;
        if(word != NULL && strcmp(word,"return") == 0) {
            temp.type=TOKEN_RETURN;
        }
        else if(word != NULL && strcmp(word,"int") == 0) {
            temp.type=TOKEN_INT;
        }
        else if(word != NULL && strcmp(word,"print") == 0) {
            temp.type=TOKEN_PRINT;
        }
        else if(word != NULL && strcmp(word,"void") == 0) {
            temp.type=TOKEN_VOID;
        }
        else{
            temp.type=TOKEN_IDENTIFIER;
        }
        return temp;
    }

    fprintf(stderr, "Unexpected character: %c\n", s);
    l->position++;
    temp.type = TOKEN_EOF;
    return temp;
}
token *tokenizer(lexer *l, int *count) {
    int capacity = 16;
    token *tokens = malloc((size_t)capacity * sizeof(token));
    if (tokens == NULL) {
        *count = 0;
        return NULL;
    }

    int i = 0;
    do {
        if (i == capacity) {
            capacity *= 2;
            token *new_tokens = realloc(tokens, (size_t)capacity * sizeof(token));
            if (new_tokens == NULL) {
                free_tokens(tokens);
                *count = 0;
                return NULL;
            }
            tokens = new_tokens;
        }
        tokens[i] = next_token(l);
    } while (tokens[i++].type != TOKEN_EOF);

    *count = i;
    return tokens;
}
void free_tokens(token *tokens) {
    if (tokens == NULL) {
        return;
    }
    for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
        free(tokens[i].txt);
    }
    free(tokens);
}
