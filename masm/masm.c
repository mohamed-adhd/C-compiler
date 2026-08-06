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
    }else if (strcmp(name, "rbx")==0) {
        return REG_RBX;
    }else if (strcmp(name, "rcx")==0){
        return REG_RCX;
    }else if (strcmp(name, "rdx")==0){
        return REG_RDX;
    }else if (strcmp(name, "rsi")==0){
        return REG_RSI;
    }else if (strcmp(name, "rdi")==0){
        return REG_RDI;
    }else if (strcmp(name, "rbp")==0){
        return REG_RDP;
    }else if (strcmp(name, "rsp") ){
        return REG_RSP;
    }
}
bool is_register_name(char* name) {
    if (strcmp(name, "rax")) {
        return true;
    }else if (strcmp(name, "rbx")==0) {
        return true;
    }else if (strcmp(name, "rcx")==0){
        return true;
    }else if (strcmp(name, "rdx")==0 ){
        return true;
    }else if (strcmp(name, "rsi")==0 ){
        return true;
    }else if (strcmp(name, "rdi")==0 ){
        return true;
    }else if (strcmp(name, "rbp")==0 ){
        return true;
    }else if (strcmp(name, "rsp")==0 ){
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

asm_line *asm_parse_program(asm_parser *p, int *out_count) {
    asm_line *lines = malloc(sizeof(asm_line) * 1024);
    int count = 0;
    while (p->tokens[p->pos].type != ASM_TOKEN_EOF) {
        if (p->tokens[p->pos].type == ASM_TOKEN_NEWLINE) {
            p->pos++;
            continue;
        }//bs
        lines[count] = asm_parse_line(p);
        count++;
        if (p->tokens[p->pos].type == ASM_TOKEN_NEWLINE) {
            p->pos++;
        }
    }
    *out_count = count;
    return lines;
}
typedef struct {
    char name[32];
    long address;
} label_entry;
int instruction_size(instruction instr) {
    const char *m = instr.mnemonic;
    if (strcmp(m, "ret") == 0) return 1;
    if (strcmp(m, "leave") == 0) return 1;
    if (strcmp(m, "syscall") == 0) return 2;
    if (strcmp(m, "push") == 0) return 1;
    if (strcmp(m, "pop") == 0) return 1;
    if (strcmp(m, "inc") == 0) return 3;
    if (strcmp(m, "dec") == 0) return 3;
    if (strcmp(m, "div") == 0) return 3;
    if (strcmp(m, "add") == 0) return 3;
    if (strcmp(m, "sub") == 0) return 3;
    if (strcmp(m, "xor") == 0) return 3;
    if (strcmp(m, "cmp") == 0) return 3;
    if (strcmp(m, "mov") == 0) {
        if (instr.op2.type == OPERAND_IMMEDIATE && instr.op1.type == OPERAND_REGISTER)
            return 7;
        if (instr.op2.type == OPERAND_REGISTER && instr.op1.type == OPERAND_REGISTER)
            return 3;
        if (instr.op1.type == OPERAND_MEMORY && instr.op2.type == OPERAND_IMMEDIATE)
            return 11;
        if (instr.op1.type == OPERAND_REGISTER && instr.op2.type == OPERAND_MEMORY)
            return 7;
        fprintf(stderr, "instruction_size is kinda wrong twin\n");
        exit(1);
    }
    if (strcmp(m, "lea") == 0) return 7;
    if (strcmp(m, "movzx") == 0) return 8;
    if (strcmp(m, "call") == 0) return 5;
    if (strcmp(m, "jne") == 0) return 6;
    if (strcmp(m, "jnz") == 0) return 6;

    fprintf(stderr, "who tf is  '%s'\n", m);
}
label_entry* pass1(asm_line *lines,int line_count){
    label_entry labels[64];
    int label_count = 0;
    long current_offset = 0;
    for(int i = 0; i < line_count; i++){
        if (lines[i].is_label) {
            strcpy(labels[label_count].name, lines[i].label_name);
            labels[label_count].address = current_offset;
            label_count++;
        } else {
            current_offset += instruction_size(lines[i].instr);
        }
    }
    return labels;
}
long rv_label(label_entry *labels, int lt, const char *name) {
    for (int i = 0; i < lt; i++) {
        if (strcmp(labels[i].name, name) == 0) {
            return labels[i].address;}}
    fprintf(stderr, "man who tf is  %s\n", name);
    exit(1);
}
void asm_encode_instruction(instruction instr, unsigned char *buf, long *offset,label_entry *labels, int label_count, long current_address) {
    if (strcmp(instr.mnemonic, "ret") == 0) {
        buf[*offset] = 0xC3;
        *offset += 1;
        return;
    }
    if (strcmp(instr.mnemonic, "syscall") == 0) {
        buf[*offset] = 0x0F;
        buf[*offset + 1] = 0x05;
        *offset += 2;
        return;
    }


}
}


