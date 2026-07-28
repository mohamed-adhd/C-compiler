
#include "lexer.h"


void lexer_init(lexer l, const char* source){
    l.line=1;
    l.source=source;
    l.position=0;
}
