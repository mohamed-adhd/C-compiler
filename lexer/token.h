typedef enum{
    TOKEN_INT,
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_COUT,
    TOKEN_RETURN,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_LPAREN,   
    TOKEN_RPAREN,   
    TOKEN_LBRACE,   
    TOKEN_RBRACE,   
    TOKEN_SEMICOLON,
    TOKEN_ver,
    TOKEN_MOD,
    TOKEN_COL,
    TOKEN_EOF
}tokentype;


typedef struct{
    tokentype type;
    char *txt;
    int val;
}token;