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
    return &temp;
}
astnode *parse_function(paarser *p) {
    astnode temp;
    temp.type=NODE_FUNCTION;
    temp.name=p->tokens[p->pos].txt;
    while(p->tokens[p->pos].type!=TOKEN_LPAREN) {
        p->pos++;
    }
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
    int op;
    for (int s;s<p->count; s++) {
        if (p->tokens[s].type==TOKEN_ADD) {
            op=1;
        }else if (p->tokens[s].type==TOKEN_SUB) {
            op=2;
        }else if (p->tokens[s].type==TOKEN_DIV) {
            op=3;
        }else if (p->tokens[s].type==TOKEN_MUL) {
            op=4;
        }else if (p->tokens[s].type==TOKEN_MOD) {
            op=5;
        }
    }
    switch (op) {
        case 1:

    }

}