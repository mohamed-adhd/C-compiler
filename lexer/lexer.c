#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


void lexer_init(lexer* l, const char* source){
    l->line=1;
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
    int s;
    while (l->source[l->position]==' ') {
        l->position++;
    }
}
token next_token(lexer *l) {
    token temp;
    char s=l->source[l->position];
    if (s==';'||s=='('||s==')'||s=='{'||s=='}'||s=='+'||s=='-'||s=='*'||s=='/') {
        switch (s) {
            case ';':
                temp.type=TOKEN_SEMICOLON;
                break;
            case '(':
                temp.type=TOKEN_LBRACE;
                break;
            case ')':
                temp.type=TOKEN_RBRACE;
                break;
            case '{':
                temp.type=TOKEN_LPAREN;
                break;
            case '}':
                temp.type=TOKEN_RPAREN;
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
        return temp;
    }else if (isdigit(s)){
        char tempo;
        int t=l->position+1;
        while (isdigit(l->source[t])) {
            tempo+=l->source[t];
            t++;
        }
        temp.type=TOKEN_NUMBER;
        temp.val=(int)tempo;
        temp.txt=&s;
        return temp;
    }else {
        char word[64];
        int i=0;
        int t=l->position+1;
        while (l->source[t]!=' ') {
            word[i]+=l->source[t];
            t++;
            i++;
        }
        if(strcmp(word,"return")) {
            temp.type=TOKEN_RETURN;
        }
        else if(strcmp(word,"int")) {
            temp.type=TOKEN_INT;
        }
        else if(strcmp(word,"print")) {
            temp.type=TOKEN_COUT;
        }
        else{
            temp.type=TOKEN_IDENTIFIER;
        }
    }
    l->position++;
    return temp;
}
token *tokenizer(lexer *l, int *count) {
    token* temp;
    temp=malloc(*count*sizeof(token));
    int i=0;
    while (l->position<strlen(l->source)) {
        temp[i]=next_token(l);
        i++;
    }
}
void free_tokens(token *tokens) {
    free(tokens);
}
