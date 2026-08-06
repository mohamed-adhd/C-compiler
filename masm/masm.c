//
// Created by bro on 8/4/26.
//
#include <stdio.h>
#include <stdlib.h>
#include "masm.h"

#include <ctype.h>
#include <stdbool.h>
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
void asm_parser_init(asm_parser *p, asm_token *tokens, int count) {
    p->tokens = tokens;
    p->pos = 0;
    p->count = count;
}
register_id register_from_name(char* name) {
    if (strcmp(name, "rax")) {
        return REG_RAX;
    }else if (strcmp(name, "rbx")) {
        return REG_RBX;
    }else if (strcmp(name, "rcx") ){
        return REG_RCX;
    }else if (strcmp(name, "rdx") ){
        return REG_RDX;
    }else if (strcmp(name, "rsi") ){
        return REG_RSI;
    }else if (strcmp(name, "rdi") ){
        return REG_RDI;
    }else if (strcmp(name, "rbp") ){
        return REG_RDP;
    }else if (strcmp(name, "rsp") ){
        return REG_RSP;
    }
}
bool is_register_name(char* name) {
    if (strcmp(name, "rax")) {
        return true;
    }else if (strcmp(name, "rbx")) {
        return true;
    }else if (strcmp(name, "rcx") ){
        return true;
    }else if (strcmp(name, "rdx") ){
        return true;
    }else if (strcmp(name, "rsi") ){
        return true;
    }else if (strcmp(name, "rdi") ){
        return true;
    }else if (strcmp(name, "rbp") ){
        return true;
    }else if (strcmp(name, "rsp") ){
        return true;
    }
    return false ;
}
operand asm_parse_operand(asm_parser *p) {
    operand op = {0};
    asm_token t = p->tokens[p->pos];
    if (t.type == ASM_TOKEN_NUMBER) {
        op.type = OPERAND_IMMEDIATE;
        op.im = t.val;
        p->pos++;
        return op;
    }

    if (t.type == ASM_TOKEN_LBRACE) {
        p->pos++;
        asm_token inner = p->tokens[p->pos];
        op.type = OPERAND_MEMORY;
        strcpy(op.label, inner.txt);
        p->pos++;
        p->pos++; // started as a c compiler , now its , whatever the fuck this is
        return op;
    }

    if (t.type == ASM_TOKEN_IDENTIFIER) {
        if (is_register_name(t.txt)) {
            op.type = OPERAND_REGISTER;
            op.reg = register_from_name(t.txt);
        } else {
            op.type = OPERAND_LABEL;
            strcpy(op.label, t.txt);
        }
        p->pos++;
        return op;
    }
    fprintf(stderr, "ay you fucked up the parser\n");
    exit(1);
}
asm_line asm_parse_line(asm_parser *p) {
    asm_line line = {0};
    asm_token first = p->tokens[p->pos];
    if (first.type == ASM_TOKEN_IDENTIFIER &&
        p->tokens[p->pos + 1].type == ASM_TOKEN_COLON) {
        line.is_label = 1;
        strcpy(line.label_name, first.txt);
        p->pos++;
        p->pos++;
        return line;
    }
    line.is_label = 0;
    strcpy(line.instr.mnemonic, first.txt);
    p->pos++;
    if (p->tokens[p->pos].type == ASM_TOKEN_NEWLINE ||p->tokens[p->pos].type == ASM_TOKEN_EOF) {
        line.instr.op1.type = OPERAND_NONE;
        line.instr.op2.type = OPERAND_NONE;
        return line;
    }
    line.instr.op1 = asm_parse_operand(p);
    if (p->tokens[p->pos].type == ASM_TOKEN_COMMA) {
        p->pos++;
        line.instr.op2 = asm_parse_operand(p);
    } else {
        line.instr.op2.type = OPERAND_NONE;
    }
    return line;
}

asm_line asm_parse_prog(asm_parser *p) {
    while (p->tokens[p->pos].type != ASM_TOKEN_EOF) {
        asm_parse_line(p);
    }
}
void encoding() {
    
}


