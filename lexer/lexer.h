#include "stddef.h"
#include "token.h"
typedef struct
{
    const char *source;
    size_t position;
    size_t line;
    size_t column;
}lexer;

void lexer_init(lexer *l, const char *source);
char peek(lexer *l);
char advance(lexer *l);
void skip_whitespace(lexer *l);
token next_token(lexer *l);
token *tokenizer(lexer *l, int *count);
void free_tokens(token *tokens);
