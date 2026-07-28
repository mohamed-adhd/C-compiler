#include "lexer.h"
#include <ctype.h>


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
    if (s==';'||s=='('||s==')'||s=='{'||s=='}') {
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
        }
        temp.txt=&s;
        return temp;
    }else if (isdigit(s)) {
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


    }
}
token *tokenize(lexer *l, int *count);
void free_tokens(token *tokens);
