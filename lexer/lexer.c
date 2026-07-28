#include "lexer.h"


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
token next_token(lexer *l);
token *tokenize(lexer *l, int *count);
void free_tokens(token *tokens);
