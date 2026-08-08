//
// Created by bro on 8/4/26.
//
#include <stdio.h>
#include <stdlib.h>
#include "masm.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "lexer.h"

void asm_lexer_init(asm_lexer *l, const char *source) {
    l->source = source;
    l->position = 0;
}
char asm_peek(asm_lexer *l) {
    return l->source[l->position];
}
char asm_advance(asm_lexer *l) {
    l->position++;
    return l->source[l->position-1];
}
void asm_skip_whitespace(asm_lexer *l) {
    while (l->source[l->position] == ' ' ||l->source[l->position] == '\t' ||l->source[l->position] == '\r') {
        l->position++;}
}
asm_token asm_next_token(asm_lexer *l) {
    asm_skip_whitespace(l);
    asm_token temp = {0};
    char s = l->source[l->position];
    if (s == '\0') {
        temp.type = ASM_TOKEN_EOF;
        return temp;
    }

    if (s==','||s=='['||s==']'||s==':'||s=='\n') {
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
    }else if (s == '"') {
        l->position++;
        char buffer[256];
        int i = 0;
        while (l->source[l->position] != '\0' &&
               l->source[l->position] != '"') {
            if (i < 255)
                buffer[i++] = l->source[l->position];
            l->position++;}
        buffer[i] = '\0';
        if (l->source[l->position] == '"')
            l->position++;
        temp.type = ASM_TOKEN_STRING;
        temp.txt = strdup(buffer);
        return temp;}
    else if (isdigit((unsigned char)s))
    {
        char buffer[64];
        int i = 0;

        while(isdigit((unsigned char)l->source[l->position]))
        {
            if(i < 63)
                buffer[i++] = l->source[l->position];

            l->position++;
        }

        buffer[i] = '\0';
        temp.type = ASM_TOKEN_NUMBER;
        temp.val = strtol(buffer,NULL,10);
        return temp;
    }else if (s == '\'')
    {
        l->position++;
        char c = l->source[l->position];
        temp.type = ASM_TOKEN_NUMBER;
        temp.val = (unsigned char)c;
        l->position++;
        if (l->source[l->position] == '\'')
            l->position++;
        return temp;
    }
    fprintf(stderr,"unknown char: '%c'\n", s);

    l->position++;

    temp.type = ASM_TOKEN_UNK;
    return temp;
}
void asm_free_tokens(asm_token *tokens) {
    if (tokens == NULL) {
        return;
    }for (int i = 0; tokens[i].type != ASM_TOKEN_EOF; i++) {
        free(tokens[i].txt);
    }
    free(tokens);
}
asm_token *asm_tokenizer(asm_lexer *l, int *count) {
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
register_id register_from_name(char *name)
{
    if (strcmp(name, "rax") == 0)
        return REG_RAX;
    if (strcmp(name, "rbx") == 0)
        return REG_RBX;






    if (strcmp(name, "rcx") == 0)
        return REG_RCX;
    if (strcmp(name, "rdx") == 0)
        return REG_RDX;
    if (strcmp(name, "rsi") == 0)
        return REG_RSI;
    if (strcmp(name, "rdi") == 0)
        return REG_RDI;
    if (strcmp(name, "rbp") == 0)
        return REG_RBP;
    if (strcmp(name, "rsp") == 0)
        return REG_RSP;
    fprintf(stderr, "who tf is '%s'\n", name);
    exit(1);
}
bool is_register_name(char *name)
{
    return strcmp(name, "rax") == 0 ||strcmp(name, "rbx") == 0 ||strcmp(name, "rcx") == 0 ||strcmp(name, "rdx") == 0 ||strcmp(name, "rsi") == 0 ||strcmp(name, "rdi") == 0 ||strcmp(name, "rbp") == 0 ||strcmp(name, "rsp") == 0;
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
    if (first.type != ASM_TOKEN_IDENTIFIER) {
        fprintf(stderr, "shi hit the fan :  %d at pos %d\n",
                first.type, p->pos);
        exit(1);
    }
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
    if (strcmp(line.instr.mnemonic,"mov")==0 ||
    strcmp(line.instr.mnemonic,"add")==0 ||
    strcmp(line.instr.mnemonic,"sub")==0 ||
    strcmp(line.instr.mnemonic,"movzx")==0)
    {
        if(strcmp(p->tokens[p->pos].txt,"byte")==0 ||
           strcmp(p->tokens[p->pos].txt,"word")==0 ||
           strcmp(p->tokens[p->pos].txt,"dword")==0 ||
           strcmp(p->tokens[p->pos].txt,"qword")==0)
        {
            p->pos++;
        }
    }
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
asm_line asm_parse_data(asm_parser *p) {
    asm_line line = {0};
    line.is_data = 1;
    line.data_size = 0;
    asm_token first = p->tokens[p->pos];
    if (strcmp(first.txt, "times") == 0) {
        p->pos++;
        int rep = p->tokens[p->pos].val;
        p->pos++;
        p->pos++;
        int val = p->tokens[p->pos].val;
        p->pos++;
        for (int i = 0; i < rep && line.data_size < 256; i++) {
            line.data_bytes[line.data_size++] = (unsigned char)val;}
        return line;}
    p->pos++;
    while (p->tokens[p->pos].type != ASM_TOKEN_NEWLINE && p->tokens[p->pos].type != ASM_TOKEN_EOF) {
        if (p->tokens[p->pos].type == ASM_TOKEN_NUMBER) {
            if (line.data_size < 256)
                line.data_bytes[line.data_size++] = (unsigned char)p->tokens[p->pos].val;
            p->pos++;
        }else if (p->tokens[p->pos].type == ASM_TOKEN_STRING) {
            char *str = p->tokens[p->pos].txt;
            for (int i = 0;str[i] != '\0' && line.data_size < 256;i++) {
                line.data_bytes[line.data_size++] =
                    (unsigned char)str[i];}
            p->pos++;
        } else if (p->tokens[p->pos].type == ASM_TOKEN_COMMA) {
            p->pos++;
        } else {
            p->pos++;}}
    return line;
}

asm_line *asm_parse_program(asm_parser *p, int *out_count) {
    asm_line *lines = malloc(sizeof(asm_line) * 1024);
    int count = 0;
    while (p->tokens[p->pos].type != ASM_TOKEN_EOF) {printf("parser pos=%d token=%d",
       p->pos,
       p->tokens[p->pos].type);
        if (p->tokens[p->pos].txt)
            printf(" text=%s\n", p->tokens[p->pos].txt);
        else
            printf(" text=NULL\n");
        if (p->tokens[p->pos].type == ASM_TOKEN_NEWLINE) {
            p->pos++;
            continue;
        }//bs
        if (strcmp(p->tokens[p->pos].txt, "BITS") == 0 ||strcmp(p->tokens[p->pos].txt, "org") == 0 ||strcmp(p->tokens[p->pos].txt, "global") == 0 ||strcmp(p->tokens[p->pos].txt, "section") == 0) {
            while (p->tokens[p->pos].type != ASM_TOKEN_NEWLINE && p->tokens[p->pos].type != ASM_TOKEN_EOF) {p->pos++;}
            continue;}
        if (p->tokens[p->pos].type == ASM_TOKEN_IDENTIFIER &&
    p->tokens[p->pos + 1].type == ASM_TOKEN_IDENTIFIER &&
    strcmp(p->tokens[p->pos + 1].txt, "db") == 0) {
            char *label = p->tokens[p->pos].txt;
            strcpy(lines[count].label_name, label);
            lines[count].is_label = 1;
            lines[count].is_data = 0;
            count++;
            p->pos++;
            lines[count] = asm_parse_data(p);
            count++;
            if (p->tokens[p->pos].type == ASM_TOKEN_NEWLINE)p->pos++;
            continue;}
        if (p->tokens[p->pos].type == ASM_TOKEN_IDENTIFIER &&
            (strcmp(p->tokens[p->pos].txt, "db") == 0 || strcmp(p->tokens[p->pos].txt, "times") == 0)) {
            lines[count] = asm_parse_data(p);
            count++;
            if (p->tokens[p->pos].type == ASM_TOKEN_NEWLINE) {p->pos++;}
            continue;}
        if (p->tokens[p->pos].type == ASM_TOKEN_IDENTIFIER &&(strcmp(p->tokens[p->pos].txt, "db") == 0 || strcmp(p->tokens[p->pos].txt, "times") == 0)) {
            while (p->tokens[p->pos].type != ASM_TOKEN_NEWLINE && p->tokens[p->pos].type != ASM_TOKEN_EOF) {p->pos++;}
            continue;}
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
    if (strcmp(m, "inc") == 0){
        if (instr.op1.type==OPERAND_MEMORY) return 6;
        return 3;
    }
    if (strcmp(m, "dec") == 0){
        if (instr.op1.type==OPERAND_MEMORY) return 6;
        return 3;
    }
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
    exit(1);
    return 0;
}
label_entry* pass1(asm_line *lines,int line_count,int *lablecnt){
    label_entry *labels = malloc(sizeof(label_entry) * 64);
    int label_count = 0;
    long current_offset = 0;
    for (int i = 0; i < line_count; i++) {
        if (lines[i].is_label) {
            strcpy(labels[label_count].name,
                   lines[i].label_name);
            labels[label_count].address = current_offset;
            label_count++;
        } else if (lines[i].is_data) {
            current_offset += lines[i].data_size;
        } else {current_offset += instruction_size(lines[i].instr);}}
    *lablecnt = label_count;
    return labels;
}

















long rv_label(label_entry *labels, int lt, const char *name) {
    for (int i = 0; i < lt; i++) {
        if (strcmp(labels[i].name, name) == 0) {
            return labels[i].address;}}
    fprintf(stderr, "man who tf is  %s\n", name);
    exit(1);
}
int get_the_gun(register_id name) {
    if (name==REG_RAX) {
        return 0;
    }else if (name==REG_RBX) {
        return 3;
    }else if (name==REG_RCX){
        return 1;
    }else if (name==REG_RDX){
        return 2;
    }else if (name==REG_RSI){
        return 6;
    }else if (name==REG_RDI){
        return 7;
    }else if (name==REG_RBP){
        return 5;
    }else if (name==REG_RSP){
        return 4;
    }
}
long compute_rip_relative(label_entry *labels, int label_count,const char *label_name, long address_after_instruction) {
    long target = rv_label(labels, label_count, label_name);
    return target - address_after_instruction;}


asm_line asm_parse_data(asm_parser *p);















void asm_encode_instruction(instruction instr, unsigned char *buf, long *offset,label_entry *labels, int label_count, long current_address) {
    if (instr.op2.type==OPERAND_NONE && instr.op1.type==OPERAND_NONE) {
        if (strcmp(instr.mnemonic, "ret") == 0) {
            buf[*offset] = 0xC3;
            *offset += 1;
            return;
        }
        if (strcmp(instr.mnemonic, "syscall") == 0) {
            buf[*offset] = 0x0F;
            buf[*offset + 1] = 0x05;
            *offset += 2;
        }

        if (strcmp(instr.mnemonic, "leave") == 0) {
            buf[*offset]=0xC9;
            *offset+=1;
            return;
        }
    }else if (instr.op2.type==OPERAND_NONE){
       if (instr.op1.im!=-5098){
           if (strcmp(instr.mnemonic, "push") == 0) {
               buf[*offset]=0x50+get_the_gun(instr.op1.reg);
               *offset+=1;
           }
           if (strcmp(instr.mnemonic, "call") == 0) {
               long target = rv_label(labels, label_count, instr.op1.label);
               int32_t rel = (int32_t)(target - (current_address + 5));

               buf[*offset] = 0xE8;
               buf[*offset + 1] = rel & 0xFF;
               buf[*offset + 2] = (rel >> 8) & 0xFF;
               buf[*offset + 3] = (rel >> 16) & 0xFF;
               buf[*offset + 4] = (rel >> 24) & 0xFF;

               *offset += 5;
               return;
           }
           if (strcmp(instr.mnemonic, "pop") == 0) {
               buf[*offset]=0x58+get_the_gun(instr.op1.reg);
               *offset+=1;
           }if (strcmp(instr.mnemonic, "inc") == 0) {
               if (instr.op1.type==OPERAND_MEMORY){
                   long disp = compute_rip_relative(labels, label_count, instr.op1.label,current_address + 6);
                   buf[*offset]=0xFE;
                   *offset+=1;
                   buf[*offset]=0x05;
                   *offset+=1;
                   buf[*offset]= disp & 0xFF;
                   buf[*offset+1]= (disp >>8) & 0xFF;
                   buf[*offset+2]= (disp >>16) & 0xFF;
                   buf[*offset+3]= (disp >>24) & 0xFF;
                   *offset+=4;
               }else{
               buf[*offset]=0x48;
               *offset+=1;
               buf[*offset]=0xFF;
               *offset+=1;
               buf[*offset]=0xC0 | get_the_gun(instr.op1.reg);
               *offset+=1;
               }
           }if (strcmp(instr.mnemonic, "dec") == 0) {
               if (instr.op1.type==OPERAND_MEMORY){
                   long disp = compute_rip_relative(labels, label_count, instr.op1.label,current_address + 6);
                   buf[*offset]=0xFE;
                   *offset+=1;
                   buf[*offset]=0x0D;
                   *offset+=1;
                   buf[*offset]= disp & 0xFF;
                   buf[*offset+1]= (disp >>8) & 0xFF;
                   buf[*offset+2]= (disp >>16) & 0xFF;
                   buf[*offset+3]= (disp >>24) & 0xFF;
                   *offset+=4;
               }else{
               buf[*offset]=0x48;
               *offset+=1;
               buf[*offset]=0xFF;
               *offset+=1;
               buf[*offset]=0xC8 | get_the_gun(instr.op1.reg);
               *offset+=1;
               }
           }if (strcmp(instr.mnemonic, "div") == 0) {
               buf[*offset]=0x48;
               *offset+=1;
               buf[*offset]=0xF7;
               *offset+=1;
               buf[*offset]=0xF0 | get_the_gun(instr.op1.reg);
               *offset+=1;
           }
    }else {
        if (strcmp(instr.mnemonic, "mov") == 0) {
            buf[*offset]= 0x48;
            buf[*offset + 1] = 0xC7;
            buf[*offset + 2] = 0xC0 | get_the_gun(instr.op1.reg);
            uint32_t imm = (uint32_t)instr.op2.im;
            buf[*offset + 3] = imm & 0xFF;
            buf[*offset + 4] = (imm >> 8) & 0xFF;
            buf[*offset + 5] = (imm >> 16) & 0xFF;
            buf[*offset + 6] = (imm >> 24) & 0xFF;

            *offset += 7;
        }
    }
}else if (strcmp(instr.mnemonic, "lea") == 0) {
    unsigned char modrm = (instr.op1.reg << 3) | 0x05;
    long disp = compute_rip_relative(labels, label_count, instr.op2.label,current_address + 7);
    buf[*offset]= 0x48;
    buf[*offset +1]=0x8D;
    buf[*offset +2] =modrm;
    buf[*offset +3]= disp & 0xFF;
    buf[*offset +4]= (disp >>8) &0xFF;
    buf[*offset +5]= (disp >>16) &0xFF;
    buf[*offset +6]= (disp >>24) &0xFF;
    *offset += 7;
}if (strcmp(instr.mnemonic, "add") == 0) {
        buf[*offset]=0x48;
        *offset+=1;
        buf[*offset]=0x01;
        *offset+=1;
        buf[*offset]=0xC0 | (get_the_gun(instr.op2.reg)*8) | get_the_gun(instr.op1.reg);
        *offset+=1;
    }
    if (strcmp(instr.mnemonic, "sub") == 0) {
        buf[*offset]=0x48;
        *offset+=1;
        buf[*offset]=0x29;
        *offset+=1;
        buf[*offset]=0xC0 | (get_the_gun(instr.op2.reg)*8) | get_the_gun(instr.op1.reg);
        *offset+=1;
    }
    if (strcmp(instr.mnemonic, "xor") == 0) {
        buf[*offset]=0x48;
        *offset+=1;
        buf[*offset]=0x31;
        *offset+=1;
        buf[*offset]=0xC0 | (get_the_gun(instr.op2.reg)*8) | get_the_gun(instr.op1.reg);
        *offset+=1;
    }
    if (strcmp(instr.mnemonic, "cmp") == 0) {
        buf[*offset]=0x48;
        *offset+=1;
        buf[*offset]=0x39;
        *offset+=1;
        buf[*offset]=0xC0 | (get_the_gun(instr.op2.reg)*8) | get_the_gun(instr.op1.reg);
        *offset+=1;}
    if (strcmp(instr.mnemonic, "mov") == 0) {
        if (instr.op1.type == OPERAND_REGISTER && instr.op2.type == OPERAND_IMMEDIATE) {
            buf[*offset]     = 0x48;
            buf[*offset + 1] = 0xC7;
            buf[*offset + 2] = 0xC0 | get_the_gun(instr.op1.reg);
            uint32_t imm = (uint32_t)instr.op2.im;
            buf[*offset + 3] = imm & 0xFF;
            buf[*offset + 4] = (imm >> 8) & 0xFF;
            buf[*offset + 5] = (imm >> 16) & 0xFF;
            buf[*offset + 6] = (imm >> 24) & 0xFF;
            *offset += 7;
        }








        else if (instr.op1.type ==OPERAND_MEMORY && instr.op2.type==OPERAND_IMMEDIATE) {
            unsigned char modrm = 0x05;
            long disp = compute_rip_relative(labels, label_count, instr.op1.label,current_address + 11);
            long imm = instr.op2.im;
            buf[*offset]     = 0x48;
            buf[*offset + 1] = 0xC7;
            buf[*offset + 2] = modrm;
            buf[*offset + 3] = disp & 0xFF;
            buf[*offset + 4] = (disp >> 8) & 0xFF;
            buf[*offset + 5] = (disp >> 16) & 0xFF;
            buf[*offset + 6] = (disp >> 24) & 0xFF;
            buf[*offset + 7] = imm & 0xFF;
            buf[*offset + 8] = (imm >> 8) & 0xFF;
            buf[*offset + 9] = (imm >> 16) & 0xFF;
            buf[*offset + 10] = (imm >> 24) & 0xFF;
            *offset += 11;
        } else if (instr.op1.type == OPERAND_REGISTER && instr.op2.type == OPERAND_MEMORY) {
            unsigned char modrm = (instr.op1.reg << 3) | 0x05;
            long disp = compute_rip_relative(labels, label_count, instr.op2.label,current_address + 7);
            buf[*offset]     = 0x48;
            buf[*offset + 1] = 0x8B;
            buf[*offset + 2] = modrm;
            buf[*offset + 3] = disp & 0xFF;
            buf[*offset + 4] = (disp >> 8) & 0xFF;
            buf[*offset + 5] = (disp >> 16) & 0xFF;
            buf[*offset + 6] = (disp >> 24) & 0xFF;
            *offset += 7;
        } else if (instr.op1.type == OPERAND_REGISTER && instr.op2.type == OPERAND_REGISTER) {
            buf[*offset]=0x48;
            *offset+=1;
            buf[*offset]=0x89;
            *offset+=1;
            buf[*offset]=0xC0 | (get_the_gun(instr.op2.reg)*8) | get_the_gun(instr.op1.reg);
            *offset+=3;
        }
    }
    if (strcmp(instr.mnemonic, "jne") == 0 || strcmp(instr.mnemonic, "jnz") == 0) {
        long target = rv_label(labels, label_count, instr.op1.label);
        long rel = target - (current_address + 6);
        buf[*offset]= 0x0F;
        buf[*offset + 1] =0x85;
        buf[*offset + 2]= rel & 0xFF;
        buf[*offset + 3]= (rel >> 8) & 0xFF;
        buf[*offset + 4]= (rel >> 16) & 0xFF;
        buf[*offset + 5]= (rel >> 24) & 0xFF;
        *offset += 6;
    }
}
void pass2(asm_line *lines,int line_count,label_entry *labels,int label_count,unsigned char *output){
    long offset = 0;
    for (int i = 0; i < line_count; i++) {
        if (lines[i].is_label)
            continue;
        if (lines[i].is_data) {
            memcpy(output + offset,lines[i].data_bytes,lines[i].data_size);
            offset += lines[i].data_size;
            continue;
        }asm_encode_instruction(lines[i].instr,output,&offset,labels,label_count,offset);}}




void avengers_assemble() {
    FILE *file = fopen("output.s", "r");
    if (file ==NULL) {
        printf("where tf is the file man");
        return;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);
    asm_lexer lexi;
    asm_lexer_init(&lexi,content);
    int token_count = 0,outie=0;

    printf("before tokenizer n shi\n");
    asm_token* tkri = asm_tokenizer(&lexi,&token_count);
    printf("after tokenizer type shi %d tokens\n",token_count);
    asm_parser parsi;
    int s=0;
    asm_parser_init(&parsi,tkri,token_count);
    printf("after parser init\n");
    asm_line* outp=asm_parse_program(&parsi,&outie);
    printf("after parser prog\n");
    label_entry* outpl=pass1(outp,outie,&s);
    printf("after pass 1\n");
    long binary_size = 0;
    for (int i = 0; i < outie; i++) {
        if (outp[i].is_label){
            continue;}
        if (outp[i].is_data)
            binary_size += outp[i].data_size;
        else
            binary_size += instruction_size(outp[i].instr);}
    unsigned char *buff = malloc(binary_size);
    pass2(outp,outie,outpl,s,buff);
    printf("after pass 2\n");
    FILE *f = fopen("output.bin", "wb");
    fwrite(buff, 1, binary_size, f);
    fclose(f);
    free(buff);
    free(outpl);

}