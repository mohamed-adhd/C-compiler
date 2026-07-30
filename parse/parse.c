#include "parse.h"

#include <ctype.h>

void parser_init(paarser *p, token *tokens, int count) {
    p->count = count;
    p->tokens = tokens;
    p->pos = 0;
}
astnode *parse_program(paarser *p) {
    astnode temp;
    temp.type=NODE_PROGRAM;
    temp.function=parse_function(p);

}
astnode *parse_function(paarser *p) {
    astnode temp;
    temp.type=NODE_FUNCTION;
    temp.name=p->tokens[p->pos].txt;
    while(p->tokens[p->pos].type!=TOKEN_LPAREN) {
        p->pos++;
    }
    p->pos++;
    temp.body=parse_statement(p);
    return &temp;
}
astnode *parse_statement(paarser *p) {
    p->pos++;
    astnode temp;
    temp.type=NODE_RETURN;
    temp.expr=parse_expression(p);
    return &temp;
}
astnode *parse_expression(paarser *p) {
    astnode temp;
    if (isdigit(p->tokens[p->pos].val)){
        temp.type=NODE_CONSTANT;
        temp.value=p->tokens[p->pos].val;
    }
    return &temp;
}