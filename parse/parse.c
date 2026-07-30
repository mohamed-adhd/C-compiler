#include "parse.h"

#include <ctype.h>

void parser_init(paarser *p, token *tokens, int count) {
    p->count = count;
    p->tokens = tokens;
    p->pos = 0;
}


astnode *parse_program(paarser *p);
astnode *parse_function(paarser *p);
astnode *parse_statement(paarser *p);
astnode *parse_expression(paarser *p) {
    astnode temp;
    if (isdigit(p->tokens[p->pos].val)){
        temp.type=NODE_CONSTANT;
        temp.value=p->tokens[p->pos].val;
    }
    return &temp;
}