#include "token.h"
#include <cstddef>



typedef struct
{
    const char *source;
    size_t position;
    size_t line;
    size_t column;
}lexer;
